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

/**
 * @brief The MaskFilesystemTraverser class
 * Represents a filesystem searcher that can return
 * list of files corresponding to some file masks
 */
class MaskFilesystemTraverser : public IFilesystemTraverser
{
public:
    MaskFilesystemTraverser(bool recursive);

    MaskFilesystemTraverser(const MaskFilesystemTraverser&) = default;
    MaskFilesystemTraverser& operator=(const MaskFilesystemTraverser&) = delete;
    MaskFilesystemTraverser(MaskFilesystemTraverser&&) = default;
    MaskFilesystemTraverser& operator=(MaskFilesystemTraverser&&) = delete;

    [[nodiscard]] FilesList get_files_list() const override;

    /**
     * @brief Add a directory to search
     * @param root_directory[in] directory to search files in
     */
    void add_root_directory(const std::filesystem::path& root_directory);

    /**
     * @brief Exclude a directory from search
     * @param exclude_directory[in] directory to exclude from search
     */
    void add_exclude_directory(const std::filesystem::path& exclude_directory);

    /**
     * @brief Add a file mask that file names must correspond to
     * This is "white-list" of file masks: if at least one is specified,
     * the class only finds files that correspond to one of these masks.
     * @param file_mask[in] File mask to add to white-list
     */
    void add_file_mask_include(std::regex file_mask);

    /**
     * @brief Add a file mask to exclude files corresponding to it
     * This is "black-list" of file masks: if a file name corresponds to at
     * least one of them, it is excluded from search
     * @param file_mask
     */
    void add_file_mask_exclude(std::regex file_mask);

    /**
     * @brief Set minimum file size to search (in bytes)
     * @param min_file_size[in] Minimum file size to search (in bytes)
     */
    void set_min_file_size(std::size_t min_file_size);

    virtual ~MaskFilesystemTraverser() = default;

private:
    void get_files_list_impl(const std::filesystem::directory_iterator& dir_it, FilesList& files_list) const;

    /**
     * @brief Check if a directory is not in exclude list
     * @param directory[in] Directory path to check
     * @return true if the directory is not in exclude list, false otherwise
     */
    bool check_directory(const std::filesystem::path& directory) const;

    /**
     * @brief Check if a file is not in black-list and
     * is in white-list (if the white-list is not empty)
     * @param file[in] File path to check
     * @return true if files corresponds the restrictions, false otherwise
     */
    bool check_file(const std::filesystem::path& file) const;

    /**
     * @brief The result of get_files_list() is cached.
     * The cache should be reset on calling every non-const member function
     */
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
