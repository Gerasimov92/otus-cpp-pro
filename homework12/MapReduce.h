#pragma once

#include <filesystem>
#include <functional>
#include <vector>

class MapReduce
{
public:
    using KeyValue = std::pair<std::string, std::size_t>;
    using MapperType = KeyValue(const std::string &);
    using ReducerType = std::size_t(const std::string &, const std::vector<std::size_t> &);

    struct DataBlock
    {
        std::uintmax_t from;
        std::uintmax_t to;
    };

    class ReduceResult
    {
    public:
        ReduceResult(MapReduce &mr);
        std::vector<std::filesystem::path> save(const std::filesystem::path &outputFolder);
    private:
        MapReduce &m_mr;
    };

    class MapResult
    {
    public:
        MapResult(MapReduce &mr);
        ReduceResult reduce(const std::function<ReducerType> &reducer);
    private:
        MapReduce &m_mr;
    };

    class LoadResult
    {
    public:
        LoadResult(MapReduce &mr);
        MapResult map(const std::function<MapperType> &mapper);
    private:
        MapReduce &m_mr;
    };

public:
    MapReduce(std::size_t mappersCount, std::size_t reducersCount);
    LoadResult load(const std::filesystem::path &inputFile);

private:
    std::vector<DataBlock> splitFile(const std::filesystem::path &input, std::size_t count);
    std::vector<std::filesystem::path> exec();
    std::vector<std::filesystem::path> map();
    void shuffle(std::vector<std::filesystem::path> mappersOutput, std::vector<std::filesystem::path> reducersInput);
    std::vector<std::filesystem::path> reduce(std::vector<std::filesystem::path> input);

    std::size_t m_mappersCount;
    std::size_t m_reducersCount;
    std::filesystem::path m_inputFile;
    std::filesystem::path m_outputFolder;
    std::function<MapperType> m_mapper;
    std::function<ReducerType> m_reducer;
};

