#ifndef CRC32_H
#define CRC32_H

#include <cstddef>

namespace my
{

struct Crc32
{
    std::size_t operator()(void* data, std::size_t length);
};

} // namespace my

#endif // CRC32_H
