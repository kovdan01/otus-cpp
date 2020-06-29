#include "utils.h"

#include <stdexcept>

namespace my
{

std::vector<std::string> split(const std::string& str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

my::ip_t splitted_to_ip(const std::vector<std::string>& splitted)
{
    if (splitted.size() != 4)
    {
        throw std::invalid_argument("IP must consist of 4 parts");
    }
    int _0 = std::stoi(splitted[0]);
    int _1 = std::stoi(splitted[1]);
    int _2 = std::stoi(splitted[2]);
    int _3 = std::stoi(splitted[3]);
    if (_0 < 0 || _0 > 255 ||
        _1 < 0 || _1 > 255 ||
        _2 < 0 || _2 > 255 ||
        _3 < 0 || _3 > 255)
    {
        throw std::invalid_argument("IP parts must be from 0 to 255");
    }
    return my::ip_t
    {
        static_cast<std::uint8_t>(_0),
        static_cast<std::uint8_t>(_1),
        static_cast<std::uint8_t>(_2),
        static_cast<std::uint8_t>(_3)
    };
}

} // namespace my
