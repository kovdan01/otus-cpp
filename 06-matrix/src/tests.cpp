#include "matrix.h"

#include <gtest/gtest.h>

#include <vector>
#include <unordered_map>

TEST(MatrixIndexing, 2D)
{
    my::Matrix<int, 2> m(5);
    m[1][1] = m[0][2] = 2;
    m[0][2] = m[100][100] = m[1][1] = 5;
    m[30][45] = (m[1][10] = 8) = 2;
    m[2][2] = 10;

    EXPECT_EQ(m[1][1], 5);
    EXPECT_EQ(m[0][2], 5);
    EXPECT_EQ(m[100][100], 5);
    EXPECT_EQ(m[1][10], 2);
    EXPECT_EQ(m[30][45], 2);
    EXPECT_EQ(m[2][2], 10);

    EXPECT_EQ(m.size(), 3);
}

TEST(MatrixAt, 2D)
{
    my::Matrix<int, 2> m(5);
    m.at({1, 1}) = m.at({0, 2}) = 2;
    m.at({0, 2}) = m.at({100, 100}) = m.at({1, 1}) = 5;
    m.at({30, 45}) = (m.at({1, 10}) = 8) = 2;
    m.at({2, 2}) = 10;

    EXPECT_EQ(m.at({1, 1}), 5);
    EXPECT_EQ(m.at({0, 2}), 5);
    EXPECT_EQ(m.at({100, 100}), 5);
    EXPECT_EQ(m.at({1, 10}), 2);
    EXPECT_EQ(m.at({30, 45}), 2);
    EXPECT_EQ(m.at({2, 2}), 10);

    EXPECT_EQ(m.size(), 3);
}

TEST(MatrixGeneral, 2D)
{
    my::Matrix<int, 2> m(0);
    std::vector<std::vector<int>> v(10, std::vector<int>(10));
    for (std::size_t i = 0; i < 10; ++i)
    {
        v[i][i] = m[i][i] = i;
        v[i][9 - i] = m[i][9 - i] = 9 - i;
    }

    for (std::size_t i = 0; i < 10; ++i)
    {
        for (std::size_t j = 0; j < 10; ++j)
        {
            EXPECT_EQ(v[i][j], m[i][j]);
            EXPECT_EQ(v[i][j], m.at({i, j}));
        }
    }

    EXPECT_EQ(m.size(), 18);
    std::map<std::array<std::size_t, 2>, int> values;

    for (const auto& elem : m)
        values.emplace(elem);

    EXPECT_EQ(m.size(), 18);
    EXPECT_EQ(values.size(), 18);

    for (std::size_t i = 0; i < 10; ++i)
    {
        if (i != 0)
        {
            auto it = values.find({i, i});
            EXPECT_NE(it, values.end());
            EXPECT_EQ(it->second, i);
        }
        if (i != 9)
        {
            auto it = values.find({i, 9 - i});
            EXPECT_NE(it, values.end());
            EXPECT_EQ(it->second, 9 - i);
        }
    }
}

TEST(MatrixGeneral, 10D)
{
    my::Matrix<int, 10> m(0);

    int& e1 = m.at({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) = 11;
    int& e2 = m.at({10, 9, 8, 7, 6, 5, 4, 3, 2, 1}) = 0;
    EXPECT_EQ(m.size(), 1);

    e1 = 0;
    e2 = 1;
    EXPECT_EQ(m.size(), 1);

    e1 = 10;
    e2 = 15;
    EXPECT_EQ(m.size(), 2);

    m.shrink_to_fit();
    EXPECT_EQ(m.size(), 2);

    e1 = e2 = 0;
    EXPECT_EQ(m.size(), 0);

    m.shrink_to_fit();
    EXPECT_EQ(m.size(), 0);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
