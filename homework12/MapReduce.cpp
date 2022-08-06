#include "MapReduce.h"

#include <fstream>
#include <set>
#include <thread>

struct MapperHandle
{
    MapperHandle() {}
    std::filesystem::path input;
    MapReduce::DataBlock block;
    std::function<MapReduce::MapperType> map;
    std::filesystem::path output;
};

struct ReducerHandle
{
    ReducerHandle() {}
    std::filesystem::path input;
    std::function<MapReduce::ReducerType> reduce;
    std::filesystem::path output;
};

static void sortMapperOutput(std::filesystem::path inputFile)
{
    std::ifstream fin(inputFile);

    std::multiset<MapReduce::KeyValue> s;
    MapReduce::KeyValue v;
    while (!fin.eof())
    {
        fin >> v.first >> v.second;
        if (!fin.fail())
            s.insert(v);
    }

    fin.close();

    std::ofstream fout(inputFile);
    for (const auto &p : s)
    {
        fout << p.first << std::endl;
        fout << p.second << std::endl;
    }
}

static void mapperWorker(MapperHandle handle)
{
    std::ifstream fin(handle.input);
    std::ofstream fout(handle.output);
    fin.seekg(handle.block.from);

    while (fin.tellg() < handle.block.to)
    {
        std::string str;
        std::getline(fin, str);
        auto res = handle.map(str);
        fout << res.first << std::endl;
        fout << res.second << std::endl;
    }

    sortMapperOutput(handle.output);
}

static void reducerWorker(ReducerHandle handle)
{
    std::ifstream fin(handle.input);
    std::ofstream fout(handle.output);

    std::vector<std::size_t> values;
    std::string lastKey;
    MapReduce::KeyValue value;

    while (!fin.eof())
    {
        fin >> value.first >> value.second;
        if (fin.fail())
            break;

        if (lastKey.empty())
            lastKey = value.first;

        if (value.first == lastKey)
        {
            values.push_back(value.second);
        }
        else
        {
            auto reducedValue = handle.reduce(lastKey, values);
            fout << lastKey << std::endl;
            fout << reducedValue << std::endl;

            values.clear();
            lastKey = value.first;
            values.push_back(value.second);
        }
    }

    if (!values.empty())
    {
        auto reducedValue = handle.reduce(lastKey, values);
        fout << lastKey << std::endl;
        fout << reducedValue << std::endl;
    }
}

MapReduce::MapReduce(std::size_t mappersCount, std::size_t reducersCount)
    : m_mappersCount(mappersCount)
    , m_reducersCount(reducersCount)
{
}

MapReduce::LoadResult MapReduce::load(const std::filesystem::path &inputFile)
{
    m_inputFile = inputFile;
    return LoadResult(*this);
}

std::vector<MapReduce::DataBlock> MapReduce::splitFile(const std::filesystem::path &input, std::size_t count)
{
    auto fileSize = std::filesystem::file_size(input);
    auto blockSize = std::filesystem::file_size(input) / count;

    if (blockSize == 0)
        return {{0, fileSize}};

    blockSize--;

    std::vector<DataBlock> blocks;
    DataBlock currentBlock{0, blockSize};
    std::ifstream fin(input);

    for (std::size_t i = 0; i < count; i++)
    {
        fin.seekg(currentBlock.to);

        char c;
        do
        {
            fin.get(c);
            if (fin.eof() || c == '\n')
                break;

            currentBlock.to++;
        }
        while (true);

        blocks.push_back(currentBlock);

        if (currentBlock.to == fileSize - 1)
            break;

        currentBlock.from = currentBlock.to + 1;
        currentBlock.to = currentBlock.from + blockSize;
        if (currentBlock.to > fileSize - 1)
            currentBlock.to = fileSize - 1;
    }

    return blocks;
}

std::vector<std::filesystem::path> MapReduce::exec()
{
    std::filesystem::create_directory(m_outputFolder);

    auto mappersOutput = map();

    std::vector<std::filesystem::path> reducersInput(m_reducersCount);
    for (std::size_t i = 0; i < reducersInput.size(); i++)
    {
        reducersInput[i] = m_outputFolder / (std::string("reducer_input_") + std::to_string(i) + std::string(".txt"));
    }

    shuffle(mappersOutput, reducersInput);

    for (const auto &path : mappersOutput)
        std::filesystem::remove(path);

    auto reducersOutput = reduce(reducersInput);

    for (const auto &path : reducersInput)
        std::filesystem::remove(path);

    return reducersOutput;
}

std::vector<std::filesystem::path> MapReduce::map()
{
    auto dataBlocks = splitFile(m_inputFile, m_mappersCount);

    std::vector<std::thread> mappers;
    mappers.reserve(dataBlocks.size());
    std::vector<MapperHandle> mapperHandles(dataBlocks.size());
    std::vector<std::filesystem::path> mappersOutput(m_mappersCount);

    for (std::size_t i = 0; i < dataBlocks.size(); i++)
    {
        mapperHandles[i].input = m_inputFile;
        mapperHandles[i].block = dataBlocks[i];
        mapperHandles[i].map = m_mapper;
        mapperHandles[i].output = m_outputFolder / (std::string("mapper_output_") + std::to_string(i) + std::string(".txt"));
        mappersOutput[i] = mapperHandles[i].output;

        mappers.emplace_back(std::thread(mapperWorker, mapperHandles[i]));
    }

    for (auto &mapper : mappers)
        mapper.join();

    return mappersOutput;
}

void MapReduce::shuffle(std::vector<std::filesystem::path> mappersOutput, std::vector<std::filesystem::path> reducersInput)
{
    std::multiset<MapReduce::KeyValue> mappersResult;
    for (const auto &path : mappersOutput)
    {
        std::ifstream fin(path);
        MapReduce::KeyValue v;
        while (!fin.eof())
        {
            fin >> v.first >> v.second;
            if (!fin.fail())
                mappersResult.insert(v);
        }
    }

    std::size_t maxCount = mappersResult.size() / reducersInput.size();
    std::size_t counter = 0;
    auto it = mappersResult.cbegin();
    std::string lastKey;

    for (std::size_t i = 0; i < reducersInput.size(); i++)
    {
        std::ofstream fout(reducersInput[i]);
        counter = 0;

        for (; it != mappersResult.cend(); ++it)
        {
            fout << it->first << std::endl;
            fout << it->second << std::endl;

            if (lastKey.empty())
                lastKey = it->first;

            counter++;

            if (counter >= maxCount)
            {
                if (it->first != lastKey)
                {
                    lastKey = it->first;
                    ++it;
                    break;
                }
            }
        }
    }
}

std::vector<std::filesystem::path> MapReduce::reduce(std::vector<std::filesystem::path> input)
{
    std::vector<std::thread> reducers;
    reducers.reserve(m_reducersCount);
    std::vector<ReducerHandle> reducerHandles(m_reducersCount);
    std::vector<std::filesystem::path> reducersOutput(m_reducersCount);

    for (std::size_t i = 0; i < m_reducersCount; i++)
    {
        reducerHandles[i].input = input[i];
        reducerHandles[i].reduce = m_reducer;
        reducerHandles[i].output = m_outputFolder / (std::string("reducer_output_") + std::to_string(i) + std::string(".txt"));
        reducersOutput[i] = reducerHandles[i].output;

        reducers.emplace_back(std::thread(reducerWorker, reducerHandles[i]));
    }

    for (auto &reducer : reducers)
        reducer.join();

    return reducersOutput;
}

MapReduce::ReduceResult::ReduceResult(MapReduce &mr)
    : m_mr(mr)
{
}

std::vector<std::filesystem::path> MapReduce::ReduceResult::save(const std::filesystem::path &outputFolder)
{
    m_mr.m_outputFolder = outputFolder;
    return m_mr.exec();
}

MapReduce::MapResult::MapResult(MapReduce &mr)
    : m_mr(mr)
{
}

MapReduce::ReduceResult MapReduce::MapResult::reduce(const std::function<ReducerType> &reducer)
{
    m_mr.m_reducer = reducer;
    return ReduceResult(m_mr);
}

MapReduce::LoadResult::LoadResult(MapReduce &mr)
    : m_mr(mr)
{
}

MapReduce::MapResult MapReduce::LoadResult::map(const std::function<MapperType> &mapper)
{
    m_mr.m_mapper = mapper;
    return MapResult(m_mr);
}
