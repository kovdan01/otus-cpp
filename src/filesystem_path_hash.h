#ifndef FILESYSTEM_PATH_HASH_H
#define FILESYSTEM_PATH_HASH_H

#include <filesystem>

namespace my
{

class FilesystemPathHash
{
public:
    std::size_t operator()(const std::filesystem::path& path) const;
};

} // namespace my

#endif // FILESYSTEM_PATH_HASH_H
