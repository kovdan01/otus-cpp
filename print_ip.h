#ifndef PRINT_IP_H
#define PRINT_IP_H

#include <type_traits>
#include <iostream>
#include <cstdint>

namespace my
{

template <typename Integer,
          typename Fake = std::enable_if_t<std::is_integral_v<Integer>, void>>
void print_ip(Integer ip, std::ostream& stream = std::cout)
{
    using UInteger = std::make_unsigned_t<Integer>;
    UInteger u_ip = *reinterpret_cast<UInteger*>(&ip);

    if constexpr (sizeof(UInteger) == 1)
    {
        int value = 0;
        for (std::size_t i = 0; i < 8; ++i)
            value += u_ip & (1 << i);
        stream << value;
        return;
    }

    std::size_t cur_shift = 8 * (sizeof(UInteger) - 1);
    UInteger mask = UInteger(255) << cur_shift;
    for (; mask != 0; mask >>= 8, cur_shift -= 8)
    {
        stream << ((mask & u_ip) >> cur_shift);
        if (cur_shift != 0)
            stream << '.';
    }
}

} // namespace my

#endif // PRINT_IP_H
