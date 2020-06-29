#include "controller.h"

#include <gtest/gtest.h>

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

struct YamrTestParam
    : public ::testing::TestWithParam<std::string>
{
    std::string filename;
};


std::vector<std::string> read_file(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string tmp;
    while (std::getline(file, tmp))
        lines.emplace_back(std::move(tmp));
    return lines;
}

std::size_t get_correct_answer(const std::string& filename)
{
    std::vector<std::string> lines = read_file(filename);
    std::size_t prefix_length = 1;
    std::vector<std::string> prefixes(lines.size());
    for (;; ++prefix_length)
    {
        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            if (lines[i].length() < prefix_length)
                return std::size_t(-1);
            prefixes[i] = lines[i].substr(0, prefix_length);
        }
        std::sort(prefixes.begin(), prefixes.end());
        auto it = std::unique(prefixes.begin(), prefixes.end());
        if (it == prefixes.end())
            break;
    }
    return prefix_length;
}

TEST_P(YamrTestParam, General)
{
    const std::string& filename = GetParam();
    std::size_t correct = get_correct_answer(filename);
    for (std::size_t mnum = 1; mnum <= 16; ++mnum)
    {
        for (std::size_t rnum = 1; rnum <= 16; ++rnum)
        {
            my::Controller controller(mnum, rnum);
            EXPECT_EQ(correct, controller.work(filename));
        }
    }
}

INSTANTIATE_TEST_CASE_P
(
    MapReduce,
    YamrTestParam,
    ::testing::Values
    (
        "data/input1.txt",
        "data/input2.txt",
        "data/input3.txt",
        "data/input4.txt"
        //"data/input5.txt"
    )
);

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
