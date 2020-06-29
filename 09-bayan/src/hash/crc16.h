#ifndef CRC16_H
#define CRC16_H

#include <cstddef>

namespace my
{

struct Crc16
{
    std::size_t operator()(void* data, std::size_t length);
};

} // namespace my

#endif // CRC16_H
