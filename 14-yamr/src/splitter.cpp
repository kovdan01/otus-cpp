#include "splitter.h"

#include <fstream>

namespace my
{

std::vector<MySplitter::FileRange> MySplitter::split(std::size_t count) const
{
    std::ifstream::pos_type size = std::ifstream(filename(), std::ios::in | std::ios::ate).tellg();
    if (size == std::ifstream::pos_type(-1))
        throw std::runtime_error("Problem while opening file");

    std::ptrdiff_t step = size / count;
    std::vector<MySplitter::FileRange> ans(count);
    std::ifstream::pos_type prev = 0, next;

    std::ifstream file(filename());

    for (std::size_t i = 0; i < count; ++i)
    {
        next = (size - prev < step ? size : prev + step);
        file.seekg(next);
        while (next < size && file.peek() != '\n')
        {
            next += 1;
            file.ignore(1);
        }
        ans[i].first = prev;
        ans[i].second = next;
        prev = next;
        if (prev != size)
            prev += 1;
    }
    return ans;
}

} // namespace my
