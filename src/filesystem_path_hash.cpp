#include "filesystem_path_hash.h"

namespace my
{

std::size_t FilesystemPathHash::operator()(const std::filesystem::path &path) const
{
    return std::filesystem::hash_value(path);
}

} // namespace my
