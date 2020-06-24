#ifndef DUPLICATE_FINDER_H
#define DUPLICATE_FINDER_H

#include "filesystem_traverser.h"
#include "filesystem_path_hash.h"

#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <tuple>
#include <fstream>
#include <string>
#include <cassert>
#include <map>

namespace my
{

/**
 * @brief The DuplicateFinder class
 * A class implementing duplicate finding logics
 * @tparam Hash function used to compare files
 */
template <typename Hash>
class DuplicateFinder
{
public:
    using Path = std::filesystem::path;

    /**
     * @brief DuplicateFinder constructor
     * @param block_size[in] Block size (in bytes) used to compare files
     */
    DuplicateFinder(std::size_t block_size);

    DuplicateFinder() = delete;
    DuplicateFinder(const DuplicateFinder&) = delete;
    DuplicateFinder& operator=(const DuplicateFinder&) = delete;
    DuplicateFinder(DuplicateFinder&&) = delete;
    DuplicateFinder& operator=(DuplicateFinder&&) = delete;

    /**
     * @brief Add filesystem traverser to search files for comparison
     * @param filesystem_traverser[in] Pointer to filesystem traverser
     */
    void add_filesystem_traverser(const IFilesystemTraverser* filesystem_traverser);

    /**
     * @brief Find duplicates in files from traversers' search
     * @return Vector of vector of path: each sub-vector contains duplicates paths
     */
    std::vector<std::vector<Path>> get_duplicates();

private:
    struct File
    {
        std::vector<std::size_t> blocks_hash;
        std::optional<std::ifstream> stream = std::nullopt;
        std::ifstream::pos_type file_size = 0;
        std::ifstream::pos_type cur_pos = 0;
        bool can_read = true;
    };

    using FileIt = typename std::unordered_map<Path, File, FilesystemPathHash>::iterator;

    std::string read_block(std::ifstream& file);
    bool compare_files(FileIt first, FileIt second);
    bool is_ended(File& file);

    Hash m_hasher;
    std::vector<const IFilesystemTraverser*> m_filesystem_traversers;
    std::unordered_map<Path, File, FilesystemPathHash> m_files;
    const std::size_t m_block_size;

};

template <typename Hash>
DuplicateFinder<Hash>::DuplicateFinder(std::size_t block_size)
    : m_hasher(Hash())
    , m_block_size(block_size)
{
}

template <typename Hash>
void DuplicateFinder<Hash>::add_filesystem_traverser(const IFilesystemTraverser* filesystem_traverser)
{
    m_filesystem_traversers.emplace_back(filesystem_traverser);
    for (Path& path : filesystem_traverser->get_files_list())
        m_files.emplace(std::move(path), File());
}

template <typename Hash>
std::string DuplicateFinder<Hash>::read_block(std::ifstream& file)
{
    std::string s(m_block_size, '\0');
    file.read(s.data(), m_block_size);
    return s;
}

template <typename Hash>
bool DuplicateFinder<Hash>::is_ended(File& file)
{
    if (!file.stream->is_open())
        return true;
    std::ifstream::pos_type p = file.stream->tellg();
    return (p == std::ifstream::pos_type(-1) || p == file.file_size);
}

template <typename Hash>
bool DuplicateFinder<Hash>::compare_files(FileIt first, FileIt second)
{
    if (first->second.file_size != second->second.file_size)
        return false;

    File *file1, *file2;
    // file1 has greater or equal number of counted hashes
    if (first->second.blocks_hash.size() > second->second.blocks_hash.size())
    {
        file1 = &(first->second);
        file2 = &(second->second);
    }
    else
    {
        file1 = &(second->second);
        file2 = &(first->second);
    }

    for (std::size_t i = 0; i < file2->blocks_hash.size(); ++i)
        if (file1->blocks_hash[i] != file2->blocks_hash[i])
            return false;

    std::string block2;
    while (file2->blocks_hash.size() < file1->blocks_hash.size())
    {
        block2 = read_block(file2->stream.value());
        file2->blocks_hash.emplace_back(m_hasher(block2.data(), block2.size()));
        if (file2->blocks_hash.back() != file1->blocks_hash[file2->blocks_hash.size() - 1])
            return false;
    }

    std::string block1;
    while (!is_ended(*file1))
    {
        block1 = read_block(file1->stream.value());
        block2 = read_block(file2->stream.value());
        file1->blocks_hash.emplace_back(m_hasher(block1.data(), block1.size()));
        file2->blocks_hash.emplace_back(m_hasher(block2.data(), block2.size()));
        if (file2->blocks_hash.back() != file1->blocks_hash.back())
            return false;
    }

    assert(is_ended(*file2));

    return true;
}

template <typename Hash>
auto DuplicateFinder<Hash>::get_duplicates() -> std::vector<std::vector<Path>>
{
    std::map<std::vector<std::size_t>, std::unordered_set<Path, FilesystemPathHash>> by_equal_hash;
    for (auto it = m_files.begin(); it != m_files.end() && std::next(it) != m_files.end(); ++it)
    {
        if (!it->second.can_read)
            continue;

        for (auto jt = std::next(it); jt != m_files.end(); ++jt)
        {
            if (!jt->second.can_read)
                continue;

            auto open_file = [](const Path& path, File& file)
            {
                if (file.stream == std::nullopt)
                {
                    file.file_size = std::ifstream(path, std::ios::in | std::ios::binary | std::ios::ate).tellg();
                    file.stream = std::ifstream(path, std::ios::in | std::ios::binary);
                }
                else
                {
                    file.stream->open(path, std::ios::in | std::ios::binary);
                    file.stream->seekg(file.cur_pos);
                }
            };

            open_file(it->first, it->second);
            open_file(jt->first, jt->second);

            if (!it->second.stream->is_open())
            {
                it->second.can_read = false;
                break;
            }
            if (!jt->second.stream->is_open())
            {
                jt->second.can_read = false;
                continue;
            }

            if (compare_files(it, jt))
            {
                auto& paths = by_equal_hash[it->second.blocks_hash];
                paths.emplace(it->first);
                paths.emplace(jt->first);
            }

            auto close_file = [](File& file)
            {
                file.cur_pos = file.stream->tellg();
                file.stream->close();
            };

            close_file(it->second);
            close_file(jt->second);
        }
    }

    std::vector<std::vector<Path>> ans;
    for (auto& [_, equal_group] : by_equal_hash)
    {
        ans.emplace_back(std::vector<Path>{});
        ans.back().reserve(equal_group.size());
        for (typename decltype (equal_group)::iterator it = equal_group.begin(), next; it != equal_group.end(); it = next)
        {
            next = std::next(it);
            ans.back().emplace_back(std::move(equal_group.extract(it).value()));
        }
    }

    return ans;
}

} // namespace my

#endif // DUPLICATE_FINDER_H
