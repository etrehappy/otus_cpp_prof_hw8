#include "arguments.h"

CommandLineArgs::CommandLineArgs(int argc, char** argv)
    :desc_{"Allowed options"}, args_containter_{}, scan_dirs_{}, exclude_dirs_{}, 
    scan_level_ {0}, min_size_{0}, block_size_{0}, file_masks_{}, hash_algorithm_{}
{
    desc_.add_options()
        ("help,h", "produce help message")
        ("scan-dirs", b_po::value<VectorPaths>()->multitoken(), "directories to scan")
        ("exclude-dirs", b_po::value<VectorPaths>()->multitoken(), "directories to exclude from scanning")
        ("scan-level", b_po::value<int>()->default_value(0), "scanning level")
        ("min-size", b_po::value<size_t>()->default_value(1), "min file's size, byte")
        ("block-size", b_po::value<size_t>()->default_value(5120), "block size for reading files")
        ("file-masks", b_po::value<std::vector<std::string>>()->multitoken(), "file masks for comparison")        
        ("hash-algorithm", b_po::value<std::string>()->default_value("md5"), "hashing algorithm (crc32, md5)");

    b_po::store(b_po::parse_command_line(argc, argv, desc_), args_containter_);
    b_po::notify(args_containter_);

    ProcessArguments(argc);
}

const VectorPaths& CommandLineArgs::GetScanDirs() const
{
    return scan_dirs_;
}

const VectorPaths& CommandLineArgs::GetExcludeDirs() const
{
    return exclude_dirs_;
}

const std::vector<std::string>& CommandLineArgs::GetMasks() const
{
    return file_masks_;
}

int CommandLineArgs::GetScanLevel() const
{
    return scan_level_;
}

size_t CommandLineArgs::GetMinSize() const
{
    return min_size_;
}

size_t CommandLineArgs::GetBlockSize() const
{
    return block_size_;
}

const std::string& CommandLineArgs::GetHashAlgorithm() const
{
    return hash_algorithm_;
}

void CommandLineArgs::ProcessArguments(int argc)
{
    if (args_containter_.count("help") || argc == 1)
    {
        desc_.print(std::cout);
        return;
    }

    scan_dirs_ = args_containter_["scan-dirs"].as<VectorPaths>();
    exclude_dirs_ = args_containter_["exclude-dirs"].as<VectorPaths>();
    scan_level_ = args_containter_["scan-level"].as<int>();
    min_size_ = args_containter_["min-size"].as<size_t>();
    block_size_ = args_containter_["block-size"].as<size_t>();
    file_masks_ = args_containter_["file-masks"].as<std::vector<std::string>>();
    hash_algorithm_ = args_containter_["hash-algorithm"].as<std::string>();
}
