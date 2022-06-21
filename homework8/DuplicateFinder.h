#pragma once

#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <set>
#include <string>
#include <vector>

namespace fs = boost::filesystem;

enum class ChecksumType
{
    CRC32,
    MD5
};

struct Config
{
    std::set<fs::path> scanDirs;
    std::set<fs::path> excludeDirs;
    std::size_t maxDepth = 0;
    std::set<boost::regex> allowedFileNameMasks;
    std::size_t minFileSize = 1;
    std::size_t blockSize = 16;
    ChecksumType checksumType = ChecksumType::CRC32;
};

struct FileInfo
{
    FileInfo(const fs::path &path, std::size_t blockSize) :
        path(path)
    {
        size = fs::file_size(path);
        std::size_t blockCount = size / blockSize;
        if (size % blockSize)
            blockCount++;
        blockChecksum.resize(blockCount);
    }
    fs::path path;
    std::size_t size;
    std::vector<std::string> blockChecksum;
    bool isProcessed = false;
};

class DuplicateFinder
{
public:
    DuplicateFinder(const Config &config);

    std::vector<std::vector<fs::path>> findDuplicates() const;

private:
    const Config m_config;

    std::vector<FileInfo> getFileList() const;
    std::vector<FileInfo> scanDir(const fs::path &dirPath, std::size_t depth) const;
    std::vector<fs::path> findDuplicatesOfFile(FileInfo &file, std::vector<FileInfo> &filesList) const;
    std::string calcChecksum(const std::vector<std::uint8_t> &data) const;
    void readBlock(std::size_t blockNum, std::ifstream &input, std::vector<std::uint8_t> &buffer) const;
    bool isEqual(FileInfo &f1, FileInfo &f2) const;
};
