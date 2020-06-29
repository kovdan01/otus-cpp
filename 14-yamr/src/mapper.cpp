#include "mapper.h"

#include <fstream>
#include <sstream>

namespace my
{

MyMapper::MyMapper(std::string filename, std::size_t prefix_length)
    : IMapper(std::move(filename))
    , m_prefix_length(prefix_length)
{
}

std::string MyMapper::read_from_file(ISplitter::FileRange range) const
{
    std::ifstream file(filename());
    file.seekg(range.first);
    std::string str(range.second - range.first, '\0');
    file.read(str.data(), range.second - range.first);
    return str;
}

std::list<std::string> MyMapper::operator()(ISplitter::FileRange range) const
{
    std::string all = read_from_file(range);
    std::string line;
    std::istringstream stream(all);
    std::list<std::string> ans;
    while (std::getline(stream, line))
    {
        if (m_prefix_length > line.size())
        {
            throw PrefixException{};
        }
        ans.emplace_back(line.substr(0, m_prefix_length));
    }
    ans.sort();
    return ans;
}

} // namespace my
