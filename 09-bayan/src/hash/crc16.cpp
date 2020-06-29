#include "crc16.h"

#include <boost/crc.hpp>

namespace my
{

std::size_t Crc16::operator()(void* data, std::size_t length)
{
    boost::crc_16_type result;
    result.process_bytes(data, length);
    return result.checksum();
}

} // namespace my
