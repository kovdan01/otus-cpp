#include "matrix.h"

#include <iostream>
#include <array>

template <std::size_t size>
std::ostream& operator<<(std::ostream& stream, const std::array<std::size_t, size>& arr)
{
    for (std::size_t elem : arr)
        stream << '[' << elem << ']';
    return stream;
}

template <typename T, std::size_t dimensions>
void iterate(const my::Matrix<T, dimensions>& m)
{
    for (const auto& [index, elem] : m)
        std::cout << index << ": " << elem << std::endl;
}

void test()
{
    my::Matrix<int, 2> m(5);
    m[1][1] = m[0][2] = 2;
    m[0][2] = m[100][100] = m[1][1] = 5;
    m[1][10] = 2;
    m[2][2] = 10;
    m.at({5, 10}) = 50;
    m.at({1, 10}) = 5;

    std::cout << "Size: " << m.size() << std::endl;
    iterate(m);

    m.shrink_to_fit();

    std::cout << "Size: " << m.size() << std::endl;
    iterate(m);
}


int main()
{
    my::Matrix<int, 2> matrix(0);
    for (std::size_t i = 0; i < 10; ++i)
    {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    for (std::size_t i = 1; i <= 8; ++i)
    {
        for (std::size_t j = 1; j <= 8; ++j)
            std::cout << matrix[i][j] << ' ';
        std::cout << '\n';
    }

    std::cout << "size = " << matrix.size() << std::endl;
    iterate(matrix);
}
