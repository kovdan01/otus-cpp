#ifndef IP_H
#define IP_H

#include <cstdint>
#include <tuple>

namespace my
{

struct ip_t
{
    std::uint8_t _0;
    std::uint8_t _1;
    std::uint8_t _2;
    std::uint8_t _3;
};

inline bool operator==(const my::ip_t& lhs, const my::ip_t& rhs)
{
    return std::tie(lhs._0, lhs._1, lhs._2, lhs._3) == std::tie(rhs._0, rhs._1, rhs._2, rhs._3);
}

} // namespace my

#endif // IP_H
