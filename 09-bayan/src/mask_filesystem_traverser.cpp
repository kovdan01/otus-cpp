#include "mask_filesystem_traverser.h"

#include <utility>
#include <algorithm>
#include <fstream>

#define UNUSED(variable) (void)variable

namespace fs = std::filesystem;
using FilesList = my::MaskFilesystemTraverser::FilesList;

namespace my
{

MaskFilesystemTraverser::MaskFilesystemTraverser(bool recursive)
    : m_recursive(recursive)
{
}

FilesList MaskFilesystemTraverser::get_files_list() const
{
    if (!m_was_run)
    {
        for (const auto& [path, iterator] : m_root_directories)
            if (check_directory(path))
                get_files_list_impl(iterator, m_files_list);
        m_was_run = true;
    }
    return m_files_list;
}

void MaskFilesystemTraverser::get_files_list_impl(const fs::directory_iterator& dir_it, FilesList& files_list) const
{
    for (const fs::directory_entry& entry : dir_it)
    {
        if (entry.is_regular_file() && check_file(entry.path()))
            files_list.emplace_back(entry.path());
        else if (m_recursive && entry.is_directory() && check_directory(entry.path()))
            get_files_list_impl(fs::directory_iterator(entry.path()), files_list);
    }
}

bool MaskFilesystemTraverser::check_file(const fs::path& file) const
{
    if (m_min_file_size != 0)
    {
        std::ifstream::pos_type file_size = std::ifstream(file, std::ios::in | std::ios::binary | std::ios::ate).tellg();
        if (file_size == -1 || static_cast<std::size_t>(file_size) < m_min_file_size)
            return false;
    }

    for (const std::regex& re : m_file_masks_exclude)
        if (std::regex_match(file.c_str(), re))
            return false;

    if (m_file_masks_include.empty())
        return true;

    for (const std::regex& re : m_file_masks_include)
        if (std::regex_match(file.c_str(), re))
            return true;

    return false;
}

bool MaskFilesystemTraverser::check_directory(const fs::path& directory) const
{
    return std::find(m_exclude_directories.begin(), m_exclude_directories.end(), directory) == m_exclude_directories.end();
}

MaskFilesystemTraverser::RootDirectory::RootDirectory(fs::path path, fs::directory_iterator iterator)
    : path(std::move(path))
    , iterator(std::move(iterator))
{
}

void MaskFilesystemTraverser::add_root_directory(const fs::path& root_directory)
{
    if (m_root_directories.find(root_directory) != m_root_directories.end())
        return;

    fs::path normal_path = root_directory.lexically_normal();

    if (m_recursive)
    {
        for (auto it = m_root_directories.begin(); it != m_root_directories.end();)
        {
            const fs::path& path = it->first;
            auto [first, second] = std::mismatch(path.begin(), path.end(), normal_path.begin(), normal_path.end());
            if (first == path.end()) // new directory is a subdir of existing directory
                return;
            if (second == normal_path.end()) // existing directory is a subdir of new directory
                it = m_root_directories.erase(it);
            else
                ++it;
            UNUSED(second);
        }
    }

    reset_cache();

    fs::directory_iterator dir_it(normal_path);
    m_root_directories.emplace(std::move(normal_path), std::move(dir_it));
}

void MaskFilesystemTraverser::reset_cache()
{
    m_was_run = false;
    m_files_list.clear();
    for (auto& [path, iterator] : m_root_directories)
        iterator = fs::directory_iterator(path);
}

void MaskFilesystemTraverser::set_min_file_size(std::size_t min_file_size)
{
    reset_cache();
    m_min_file_size = min_file_size;
}

void MaskFilesystemTraverser::add_exclude_directory(const fs::path& exclude_directory)
{
    reset_cache();
    m_exclude_directories.emplace_back(exclude_directory.lexically_normal());
}

void MaskFilesystemTraverser::add_file_mask_include(std::regex file_mask)
{
    reset_cache();
    m_file_masks_include.emplace_back(std::move(file_mask));
}

void MaskFilesystemTraverser::add_file_mask_exclude(std::regex file_mask)
{
    reset_cache();
    m_file_masks_exclude.emplace_back(std::move(file_mask));
}

} // namespace my
