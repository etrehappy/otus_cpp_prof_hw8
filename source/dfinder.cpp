#include "dfinder.h"

DuplicateFinder::DuplicateFinder(const CommandLineArgs& args)
    :files_paths_{}, duplicate_groups_{}, args_{args}
{
}


void DuplicateFinder::Find()
{
   Reset();

    for (const auto& dir_path : args_.GetScanDirs())
    {
        SearchFiles(dir_path, args_.GetScanLevel());
    }

    SearchDuplicate();
}

void DuplicateFinder::PrintGroups() 
{
    for (const auto& group : duplicate_groups_) 
    {
        if (group.second.size() > 1) //print only duplicates
        {
            for (const auto& path : group.second) 
            {
                std::cout << path.string() << "\n";
            }
            std::cout << std::endl; 
        }
    }
}


void DuplicateFinder::Reset()
{
    files_paths_.clear();
    duplicate_groups_.clear();
}

void DuplicateFinder::SearchFiles(const b_fs::path& dir, const int level)
{
    if (level < 0) { return; }

    b_fs::directory_iterator end{};

    for (b_fs::directory_iterator it(dir); it != end; ++it) 
    {        
        if (IsDirExclude(it)) { continue; }       

        if (b_fs::is_directory(it->status())) 
        {
            SearchFiles(it->path(), level - 1);
        }
        else if (IsRegularFileWithMinSize(it))
        {
            if (!IsFileMatchMask(it)) { continue; }

            AddFilePathToContainer(it);
        }
    }
}

bool DuplicateFinder::IsDirExclude(const b_fs::directory_iterator& it) const
{
    auto current_dir = std::find(args_.GetExcludeDirs().begin(), 
        args_.GetExcludeDirs().end(), it->path());

    return current_dir != args_.GetExcludeDirs().end();
}

bool DuplicateFinder::IsRegularFileWithMinSize(const b_fs::directory_iterator& it) const
{
    return (b_fs::is_regular_file(it->status()) &&
        b_fs::file_size(it->path()) >= args_.GetMinSize());
}

bool DuplicateFinder::IsFileMatchMask(const b_fs::directory_iterator& it) const
{
    for (const auto& mask : args_.GetMasks())
    {
        if (it->path().filename().string().find(mask) != std::string::npos)
        {
            return true;
        }
    }

    return false;
}

inline void DuplicateFinder::AddFilePathToContainer(const b_fs::directory_iterator& it)
{       
    files_paths_.push_back(it->path());
}

void DuplicateFinder::SearchDuplicate()
{
    size_t group_id = 0;

    for (const auto& file_path : files_paths_)
    {
        bool duplicate_found = false;

        for (auto& group : duplicate_groups_)
        {
            //Compare the current file with the first file in the duplicate group
            if (CompareFiles(file_path, group.second[0]))
            {
                duplicate_found = true;
                group.second.push_back(file_path);                
                break;
            }
        }

        if (!duplicate_found)
        {
            //Create a new group for the current file
            duplicate_groups_[group_id++].push_back(file_path);
        }
    }
}

bool DuplicateFinder::CompareFiles(const b_fs::path& file1, const b_fs::path& file2) const
{
    b_io::mapped_file_source f1(file1), f2(file2);

    if (!f1.is_open() || !f2.is_open()) 
    {
        throw std::runtime_error("Error comparison: unable to open files.");
    }

    const size_t size{f1.size()}, max_block_size{args_.GetBlockSize()};

    if (size != f2.size()) { return false; }
   

    for (size_t offset = 0; offset < size; offset += max_block_size)
    {
        std::string block1{}, block2{};

        FillBloks(block1, block2, f1.data(), f2.data(), max_block_size, size, offset);

        if (!IsBlocksEqual(block1, block2)) { return false; };
    }

    return true;
}

void DuplicateFinder::FillBloks(std::string& block1, std::string& block2,
    const char* start_pos_1, const char* start_pos_2,
    const size_t max_block_size, const size_t block_size, const size_t offset) const

{   
    size_t bytes_to_read = std::min(max_block_size, block_size - offset);

    block1 = std::string{start_pos_1 + offset, bytes_to_read};
    block2 = std::string{start_pos_2 + offset, bytes_to_read};

    if (block1.size() < max_block_size) // block1.size() == block2.size()
    {
        size_t padding_size = max_block_size - block1.size();
        block1.append(padding_size, '\0');
        block2.append(padding_size, '\0');
    }
}

bool DuplicateFinder::IsBlocksEqual(std::string& block1, std::string& block2) const
{
    return (HashBlock(block1) == HashBlock(block2));
}

 std::string DuplicateFinder::HashBlock(const std::string& block) const
{
     const auto& algorithm = args_.GetHashAlgorithm();

    if (algorithm == "crc32")
    {
        boost::crc_32_type crc32{};
        crc32.process_bytes(block.data(), block.size());
        return std::to_string(crc32.checksum());
    }
    else if (algorithm == "md5")
    {
        boost::uuids::detail::md5 md5{};
        boost::uuids::detail::md5::digest_type digest{};
        md5.process_bytes(block.data(), block.size());
        md5.get_digest(digest);
        const auto char_digest = reinterpret_cast<const char*>(&digest);
        return std::string(char_digest, char_digest + sizeof(boost::uuids::detail::md5::digest_type));
    }

    throw std::invalid_argument("Unknown algorithm: " + algorithm);
}




