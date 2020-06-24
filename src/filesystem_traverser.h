#ifndef FILESYSTEM_TRAVERSER_H
#define FILESYSTEM_TRAVERSER_H

#include <vector>
#include <filesystem>

namespace my
{

/**
 * @brief The IFilesystemTraverser interface
 * Describes an object that can return files list
 */
class IFilesystemTraverser
{
public:
    using FilesList = std::vector<std::filesystem::path>;

    [[nodiscard]] virtual FilesList get_files_list() const = 0;

    virtual ~IFilesystemTraverser() = default;
};

} // namespace my

#endif // FILESYSTEM_TRAVERSER_H
