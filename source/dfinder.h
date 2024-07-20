#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/crc.hpp> 
#include <boost/uuid/detail/md5.hpp> 
#include <boost/container/flat_map.hpp>
#include <boost/container/vector.hpp>

#include <iostream>

#include "arguments.h"

namespace b_fs = boost::filesystem;
namespace b_io = boost::iostreams;
namespace b_cntr = boost::container;

class DuplicateFinder
{
public:
    DuplicateFinder(const CommandLineArgs&);
    ~DuplicateFinder() = default;

    void Find();
    void PrintGroups();

private:

    void Reset();
    void SearchFiles(const b_fs::path&, const int);
    bool IsDirExclude(const b_fs::directory_iterator&) const;
    bool IsRegularFileWithMinSize(const b_fs::directory_iterator&) const;
    bool IsFileMatchMask(const b_fs::directory_iterator&) const;
    void AddFilePathToContainer(const b_fs::directory_iterator&);

    void SearchDuplicate();
    bool CompareFiles(const b_fs::path&, const b_fs::path&) const;
    void FillBloks(std::string& block1, std::string& block2, const char* start_pos_1, const char* start_pos_2,
        const size_t max_block_size, const size_t size, const size_t offset) const;
    bool IsBlocksEqual(std::string&, std::string&) const;
    std::string HashBlock(const std::string&) const;
        
    b_cntr::vector<b_fs::path> files_paths_;
    b_cntr::flat_map<size_t, b_cntr::vector<b_fs::path>> duplicate_groups_;
    const CommandLineArgs& args_;
};

