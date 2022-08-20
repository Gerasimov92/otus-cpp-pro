#include "MapReduce.h"
#include <numeric>
#include <iostream>
#include <fstream>

bool analyzeResult(const std::vector<std::filesystem::path> &v)
{
    for (const auto &path : v)
    {
        std::ifstream fin(path);
        MapReduce::KeyValue v;
        while (!fin.eof())
        {
            fin >> v.first >> v.second;
            if (fin.fail())
                break;

            if (v.second > 1)
                return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: mapreduce <inputFile> <mappersCount> <reducersCount>" << std::endl;
        return 1;
    }

    std::filesystem::path inputFile(argv[1]);
    std::filesystem::path outputFolder = inputFile.parent_path() / "out";

    int mappersCount = std::atoi(argv[2]);
    if (mappersCount <= 0)
        mappersCount = 1;

    int reducersCount = std::atoi(argv[3]);
    if (reducersCount <= 0)
        reducersCount = 1;

    auto reducer = [](const std::string &key, const std::vector<std::size_t> &values) {
        return std::accumulate(values.cbegin(), values.cend(), 0);
    };

    std::filesystem::remove_all(outputFolder);

    for (int i = 1; i < 256; i++)
    {
        MapReduce mr(mappersCount, reducersCount);
        auto result = mr
            .load(inputFile)
            .map([i](const std::string &key) { return std::pair{key.substr(0, i), 1}; })
            .reduce(reducer)
            .save(outputFolder);

        if (analyzeResult(result))
        {
            std::cout << "Minimum prefix size = " << i << std::endl;
            return 0;
        }

        for (const auto &path : result)
            std::filesystem::remove(path);
    }

    std::cout << "Minimum prefix not found" << std::endl;

    return 0;
}
