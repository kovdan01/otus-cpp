#include "matrix.h"

#include <iostream>
#include <array>
#include <iomanip>

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
    std::size_t i = 0;
    for (const auto& [index, elem] : m)
    {
        std::cout << std::setw(2) << i << ": " << index << ": " << elem << std::endl;
        ++i;
    }
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
