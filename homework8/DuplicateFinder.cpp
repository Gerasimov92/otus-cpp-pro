#include "DuplicateFinder.h"

#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>

#include <fstream>
#include <sstream>

using boost::uuids::detail::md5;

template<typename T>
static std::string toString(const T &value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

static std::string toString(const md5::digest_type &digest)
{
    const auto intDigest = reinterpret_cast<const int*>(&digest);
    std::string result;
    boost::algorithm::hex(intDigest, intDigest + (sizeof(md5::digest_type) / sizeof(int)), std::back_inserter(result));
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return std::tolower(c); });
    return result;
}

DuplicateFinder::DuplicateFinder(const Config &config) :
    m_config(config)
{}

std::vector<std::vector<fs::path> > DuplicateFinder::findDuplicates() const
{
    auto files = getFileList();

    std::vector<std::vector<fs::path>> result;

    for (auto &f : files)
    {
        if (f.isProcessed)
            continue;

        auto duplicates = findDuplicatesOfFile(f, files);
        if (!duplicates.empty())
        {
            duplicates.push_back(f.path);
            result.push_back(duplicates);
        }
    }

    return result;
}

std::vector<FileInfo> DuplicateFinder::getFileList() const
{
    std::vector<FileInfo> result;

    for (const auto &dir : m_config.scanDirs)
    {
        auto files = scanDir(dir, m_config.maxDepth);
        result.insert(result.end(), files.begin(), files.end());
    }

    return result;
}

std::vector<FileInfo> DuplicateFinder::scanDir(const fs::path &dirPath, std::size_t depth) const
{
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
        return {};

    if (m_config.excludeDirs.count(dirPath))
        return {};

    std::vector<FileInfo> files;

    for (auto &path : fs::directory_iterator(dirPath))
    {
        if (fs::is_regular_file(path))
        {
            FileInfo info(path, m_config.blockSize);
            if (info.size < m_config.minFileSize)
                continue;

            bool isAllowed = false;

            for (const auto &re : m_config.allowedFileNameMasks)
            {
                boost::smatch match;
                if (boost::regex_match(info.path.filename().string(), match, re))
                {
                    isAllowed = true;
                    break;
                }
            }

            if (isAllowed || m_config.allowedFileNameMasks.empty())
                files.push_back(info);
        }
        else if (fs::is_directory(path))
        {
            if (depth > 0)
            {
                auto subfolderFiles = scanDir(path, depth - 1);
                files.insert(files.end(), subfolderFiles.begin(), subfolderFiles.end());
            }
        }
    }

    return files;
}

std::vector<fs::path> DuplicateFinder::findDuplicatesOfFile(FileInfo &file, std::vector<FileInfo> &filesList) const
{
    file.isProcessed = true;
    std::vector<fs::path> duplicates;

    for (auto &otherFile : filesList)
    {
        if (file.path == otherFile.path)
            continue;

        if (isEqual(file, otherFile))
        {
            otherFile.isProcessed = true;
            duplicates.push_back(otherFile.path);
        }
    }

    return duplicates;
}

std::string DuplicateFinder::calcChecksum(const std::vector<std::uint8_t> &data) const
{
    if (m_config.checksumType == ChecksumType::CRC32)
    {
        boost::crc_32_type crc;
        crc.process_bytes(data.data(), data.size());
        return toString(crc.checksum());
    }
    else if (m_config.checksumType == ChecksumType::MD5)
    {
        md5 hash;
        md5::digest_type digest;
        hash.process_bytes(data.data(), data.size());
        hash.get_digest(digest);
        return toString(digest);
    }
    else
    {
        return "";
    }
}

void DuplicateFinder::readBlock(std::size_t blockNum, std::ifstream &input, std::vector<uint8_t> &buffer) const
{
    std::fill(buffer.begin(), buffer.end(), 0);
    input.seekg(blockNum * m_config.blockSize);
    input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
}

bool DuplicateFinder::isEqual(FileInfo &f1, FileInfo &f2) const
{
    if (f1.size != f2.size)
        return false;

    std::vector<std::uint8_t> buffer(m_config.blockSize);
    std::ifstream f1_in(f1.path, std::ios::binary);
    std::ifstream f2_in(f2.path, std::ios::binary);

    for (std::size_t i = 0; i < f1.blockChecksum.size(); i++)
    {
        if (f1.blockChecksum[i].empty())
        {
            readBlock(i, f1_in, buffer);
            f1.blockChecksum[i] = calcChecksum(buffer);
        }

        if (f2.blockChecksum[i].empty())
        {
            readBlock(i, f2_in, buffer);
            f2.blockChecksum[i] = calcChecksum(buffer);
        }

        if (f1.blockChecksum[i] != f2.blockChecksum[i])
            return false;
    }

    return true;
}
