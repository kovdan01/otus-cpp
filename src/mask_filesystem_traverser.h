#ifndef MASK_FILESYSTEM_TRAVERSER_H
#define MASK_FILESYSTEM_TRAVERSER_H

#include "filesystem_traverser.h"
#include "filesystem_path_hash.h"

#include <filesystem>
#include <regex>
#include <vector>
#include <unordered_map>

namespace my
{

class MaskFilesystemTraverser : public IFilesystemTraverser
{
public:
    MaskFilesystemTraverser(bool recursive);

    MaskFilesystemTraverser(const MaskFilesystemTraverser&) = default;
    MaskFilesystemTraverser& operator=(const MaskFilesystemTraverser&) = delete;
    MaskFilesystemTraverser(MaskFilesystemTraverser&&) = default;
    MaskFilesystemTraverser& operator=(MaskFilesystemTraverser&&) = delete;

    [[nodiscard]] FilesList get_files_list() const override;

    void add_root_directory(const std::filesystem::path& root_directory);
    void add_exclude_directory(const std::filesystem::path& exclude_directory);
    void add_file_mask_include(std::regex file_mask);
    void add_file_mask_exclude(std::regex file_mask);
    void set_min_file_size(std::size_t min_file_size);

    virtual ~MaskFilesystemTraverser() = default;

private:
    void get_files_list_impl(const std::filesystem::directory_iterator& dir_it, FilesList& files_list) const;
    bool check_directory(const std::filesystem::path& directory) const;
    bool check_file(const std::filesystem::path& file) const;
    void reset_cache();

    struct RootDirectory
    {
        RootDirectory(std::filesystem::path path, std::filesystem::directory_iterator iterator);

        std::filesystem::path path;
        std::filesystem::directory_iterator iterator;
    };

    std::unordered_map<std::filesystem::path, std::filesystem::directory_iterator, FilesystemPathHash> m_root_directories;
    std::vector<std::filesystem::path> m_exclude_directories;
    std::vector<std::regex> m_file_masks_include;
    std::vector<std::regex> m_file_masks_exclude;

    std::size_t m_min_file_size = 0;

    mutable FilesList m_files_list;
    mutable bool m_was_run = false;

    const bool m_recursive;
};

} // namespace my

#endif // MASK_FILESYSTEM_TRAVERSER_H
