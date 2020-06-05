#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <cstdint>
#include <utility>

struct ip_t
{
    std::uint8_t _0;
    std::uint8_t _1;
    std::uint8_t _2;
    std::uint8_t _3;
};

using ip_pool_t = std::vector<ip_t>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
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

ip_t splitted_to_ip(const std::vector<std::string>& splitted)
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
    return ip_t
    {
        static_cast<std::uint8_t>(_0),
        static_cast<std::uint8_t>(_1),
        static_cast<std::uint8_t>(_2),
        static_cast<std::uint8_t>(_3)
    };
}

template <typename T>
class Filter
{
public:
    Filter(const std::vector<T>& pool,
           std::function<bool(const T&)> predicate)
        : m_pool(pool)
        , m_predicate(std::move(predicate))
    {
    }

    class Iterator
    {
    public:
        Iterator(typename std::vector<T>::const_iterator it, const Filter* filter)
            : m_it(it)
            , m_filter(filter)
        {
            while (m_it != m_filter->m_pool.cend() && !m_filter->m_predicate(*m_it))
            {
                ++m_it;
            }
        }

        Iterator& operator++()
        {
            ++m_it;
            while (m_it != m_filter->m_pool.cend() && !m_filter->m_predicate(*m_it))
            {
                ++m_it;
            }
            return *this;
        }

        const T& operator*() const
        {
            return *m_it;
        }

        bool operator!=(const Iterator& other)
        {
            return m_it != other.m_it;
        }

    private:
        typename std::vector<T>::const_iterator m_it;
        const Filter* m_filter;
    };

    Iterator begin() const
    {
        return Iterator{m_pool.cbegin(), this};
    }

    Iterator end() const
    {
        return Iterator{m_pool.cend(), this};
    }

private:
    const std::vector<T>& m_pool;
    std::function<bool(const T&)> m_predicate;
};

template <typename IpPool>
void print_pool(const IpPool& ip_pool)
{
    for (const ip_t& ip : ip_pool)
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
        ip_pool_t ip_pool;

        for (std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(splitted_to_ip(split(v.at(0), '.')));
        }

        std::sort(ip_pool.begin(), ip_pool.end(), [](const ip_t& lhs, const ip_t& rhs)
        {
            return std::tie(lhs._0, lhs._1, lhs._2, lhs._3) > std::tie(rhs._0, rhs._1, rhs._2, rhs._3);
        });

        Filter<ip_t> filter1(ip_pool, [](const ip_t& ip)
        {
            return (ip._0 == 1);
        });

        Filter<ip_t> filter46_70(ip_pool, [](const ip_t& ip)
        {
            return (ip._0 == 46 && ip._1 == 70);
        });

        Filter<ip_t> filter46_any(ip_pool, [](const ip_t& ip)
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
