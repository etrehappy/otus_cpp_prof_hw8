#pragma once

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <vector>
#include <string>
#include <iostream>

namespace b_po = boost::program_options;
namespace b_fs = boost::filesystem;
using VectorPaths = std::vector<b_fs::path>;

class CommandLineArgs
{
public:
    CommandLineArgs(int argc, char** argv);
    ~CommandLineArgs() = default;
    
    const VectorPaths& GetScanDirs() const;
    const VectorPaths& GetExcludeDirs() const;
    const std::vector<std::string>& GetMasks() const;
    int GetScanLevel() const;
    size_t GetMinSize() const;
    size_t GetBlockSize() const;
    const std::string& GetHashAlgorithm() const;

private:
    void ProcessArguments(int argc);

    b_po::options_description desc_;
    b_po::variables_map args_containter_;

    VectorPaths scan_dirs_, exclude_dirs_;
    int scan_level_;
    size_t min_size_, block_size_;
    std::vector<std::string> file_masks_;
    std::string hash_algorithm_;
};
