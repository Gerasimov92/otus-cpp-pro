#include "DuplicateFinder.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    Config config;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("scanDir", po::value<std::vector<std::string>>(), "directory to scan")
        ("excludeDir", po::value<std::vector<std::string>>(), "exclude directory")
        ("maxDepth", po::value<std::size_t>(&config.maxDepth)->default_value(0), "max scan depth")
        ("fileMask", po::value<std::vector<std::string>>()->composing(), "allowed filename mask/regex (if not specified, then all files allowed)")
        ("minFileSize", po::value<std::size_t>(&config.minFileSize)->default_value(1), "minimum file size")
        ("blockSize", po::value<std::size_t>(&config.blockSize)->default_value(16), "block size")
        ("checksumType", po::value<std::string>()->default_value("CRC32"), "checksum type (CRC32 or MD5)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("scanDir"))
    {
        auto scanDirs = vm["scanDir"].as<std::vector<std::string>>();
        for (const auto &dir : scanDirs)
        {
            auto path = fs::weakly_canonical(dir);

            bool insert = true;
            for (const auto &cpath : config.scanDirs)
            {
                if (path.string().find(cpath.string()) == 0)
                {
                    insert = false;
                    break;
                }
            }

            if (insert)
            {
                for (auto it = config.scanDirs.begin(); it != config.scanDirs.end(); )
                {
                    if(it->string().find(path.string()) == 0)
                        it = config.scanDirs.erase(it);
                    else
                        ++it;
                }

                config.scanDirs.insert(path);
            }
        }
    }

    if (vm.count("excludeDir"))
    {
        auto excludeDirs = vm["excludeDir"].as<std::vector<std::string>>();
        for (const auto &dir : excludeDirs)
            config.excludeDirs.insert(fs::weakly_canonical(dir));
    }

    if (vm.count("fileMask"))
    {
        auto fileMasks = vm["fileMask"].as<std::vector<std::string>>();
        for (const auto &m : fileMasks)
            config.allowedFileNameMasks.insert(boost::regex(m));
    }

    auto checksumType = vm["checksumType"].as<std::string>();
    if (checksumType == "CRC32")
        config.checksumType = ChecksumType::CRC32;
    else if (checksumType == "MD5")
        config.checksumType = ChecksumType::MD5;

    DuplicateFinder finder(config);
    auto duplicates = finder.findDuplicates();

    for (const auto &d : duplicates)
    {
        for (const auto &path : d)
            std::cout << path.string() << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
