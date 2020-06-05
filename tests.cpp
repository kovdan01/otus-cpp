#include "ip.h"
#include "utils.h"
#include "input.h"
#include "filter.h"

#include <gtest/gtest.h>

#include <string>
#include <functional>
#include <sstream>
#include <tuple>
#include <iostream>
#include <algorithm>

struct IpFilterParam
    : public ::testing::TestWithParam
    <
        std::tuple
        <
            std::string,
            std::function<bool(const my::ip_t&)>,
            std::vector<my::ip_t>
        >
    >
{
    std::string raw_ips;
    std::function<bool(const my::ip_t&)> predicate;
    std::vector<my::ip_t> correct;
};

TEST_P(IpFilterParam, CheckFilter)
{
    const std::string& raw_ips = std::get<0>(GetParam());
    const std::function<bool(const my::ip_t&)>& predicate = std::get<1>(GetParam());
    const std::vector<my::ip_t> correct = std::get<2>(GetParam());

    // prepare data
    std::vector<my::ip_t> ip_pool;
    std::istringstream sstream(raw_ips);
    for (std::string line; std::getline(sstream, line);)
    {
        std::vector<std::string> v = my::split(line, '\t');
        ip_pool.push_back(my::splitted_to_ip(my::split(v.at(0), '.')));
    }
    std::sort(ip_pool.begin(), ip_pool.end(), [](const my::ip_t& lhs, const my::ip_t& rhs)
    {
        return std::tie(lhs._0, lhs._1, lhs._2, lhs._3) > std::tie(rhs._0, rhs._1, rhs._2, rhs._3);
    });

    // test
    my::Filter<my::ip_t> filter(ip_pool, predicate);
    std::vector<my::ip_t> filtered;
    for (const my::ip_t& ip : filter)
    {
        filtered.push_back(ip);
    }

    ASSERT_EQ(filtered.size(), correct.size());
    for (std::size_t i = 0; i < correct.size(); ++i)
    {
        ASSERT_EQ(filtered[i], correct[i]);
    }
}

INSTANTIATE_TEST_CASE_P
(
    Filter,
    IpFilterParam,
    ::testing::Values
    (
        std::make_tuple
        (
            IP_LIST,
            [](const my::ip_t& ip)
            {
                return (ip._0 == 1);
            },
            std::vector<my::ip_t>
            {
                {   1, 231,  69,  33 },
                {   1,  87, 203, 225 },
                {   1,  70,  44, 170 },
                {   1,  29, 168, 152 },
                {   1,   1, 234,   8 }
            }
        ),
        std::make_tuple
        (
            IP_LIST,
            [](const my::ip_t& ip)
            {
                return (ip._0 == 46 && ip._1 == 70);
            },
            std::vector<my::ip_t>
            {
                {  46,  70, 225,  39 },
                {  46,  70, 147,  26 },
                {  46,  70, 113,  73 },
                {  46,  70,  29,  76 }
            }
        ),
        std::make_tuple
        (
            IP_LIST,
            [](const my::ip_t& ip)
            {
                return (ip._0 == 46 || ip._1 == 46 || ip._2 == 46 || ip._3 == 46);
            },
            std::vector<my::ip_t>
            {
                { 186, 204,  34,  46 },
                { 186,  46, 222, 194 },
                { 185,  46,  87, 231 },
                { 185,  46,  86, 132 },
                { 185,  46,  86, 131 },
                { 185,  46,  86, 131 },
                { 185,  46,  86,  22 },
                { 185,  46,  85, 204 },
                { 185,  46,  85,  78 },
                {  68,  46, 218, 208 },
                {  46, 251, 197,  23 },
                {  46, 223, 254,  56 },
                {  46, 223, 254,  56 },
                {  46, 182,  19, 219 },
                {  46, 161,  63,  66 },
                {  46, 161,  61,  51 },
                {  46, 161,  60,  92 },
                {  46, 161,  60,  35 },
                {  46, 161,  58, 202 },
                {  46, 161,  56, 241 },
                {  46, 161,  56, 203 },
                {  46, 161,  56, 174 },
                {  46, 161,  56, 106 },
                {  46, 161,  56, 106 },
                {  46, 101, 163, 119 },
                {  46, 101, 127, 145 },
                {  46,  70, 225,  39 },
                {  46,  70, 147,  26 },
                {  46,  70, 113,  73 },
                {  46,  70,  29,  76 },
                {  46,  55,  46,  98 },
                {  46,  49,  43,  85 },
                {  39,  46,  86,  85 },
                {   5, 189, 203,  46 }
            }
        )
    ),
);

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
