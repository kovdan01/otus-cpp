#include "crc32.h"

#include <boost/crc.hpp>

namespace my
{

std::size_t Crc32::operator()(void* data, std::size_t length)
{
    boost::crc_32_type result;
    result.process_bytes(data, length);
    return result.checksum();
}

} // namespace my
