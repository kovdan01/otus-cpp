#include "filter.h"
#include "ip.h"
#include "utils.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <cstdint>
#include <utility>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]


template <typename IpPool>
void print_pool(const IpPool& ip_pool)
{
    for (const my::ip_t& ip : ip_pool)
    {
        std::cout << static_cast<int>(ip._0) << '.'
                  << static_cast<int>(ip._1) << '.'
                  << static_cast<int>(ip._2) << '.'
                  << static_cast<int>(ip._3) << '\n';
    }
}

int main(int argc, char const* argv[])
{
    try
    {
        std::vector<my::ip_t> ip_pool;

        for (std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = my::split(line, '\t');
            ip_pool.push_back(my::splitted_to_ip(my::split(v.at(0), '.')));
        }

        std::sort(ip_pool.begin(), ip_pool.end(), [](const my::ip_t& lhs, const my::ip_t& rhs)
        {
            return std::tie(lhs._0, lhs._1, lhs._2, lhs._3) > std::tie(rhs._0, rhs._1, rhs._2, rhs._3);
        });

        my::Filter<my::ip_t> filter1(ip_pool, [](const my::ip_t& ip)
        {
            return (ip._0 == 1);
        });

        my::Filter<my::ip_t> filter46_70(ip_pool, [](const my::ip_t& ip)
        {
            return (ip._0 == 46 && ip._1 == 70);
        });

        my::Filter<my::ip_t> filter46_any(ip_pool, [](const my::ip_t& ip)
        {
            return (ip._0 == 46 || ip._1 == 46 || ip._2 == 46 || ip._3 == 46);
        });

        print_pool(ip_pool);
        print_pool(filter1);
        print_pool(filter46_70);
        print_pool(filter46_any);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
