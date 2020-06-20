#include "mapper.h"

#include <fstream>
#include <sstream>

namespace my
{

namespace
{

std::string read_from_file(const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to)
{
    std::ifstream file(filename);
    file.seekg(from);
    std::string str(to - from, '\0');
    file.read(str.data(), to - from);
    return str;
}

} // namespace

std::pair<std::list<std::string>, bool> mapper(const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to, std::size_t prefix_size)
{
    std::string all = read_from_file(filename, from, to);
    std::string line;
    std::istringstream stream(all);
    std::list<std::string> ans;
    while (std::getline(stream, line))
    {
        //ans.emplace_back(line);
        if (prefix_size > line.size())
        {
            return {{}, false};
        }
        ans.emplace_back(line.substr(0, prefix_size));
    }
    return {std::move(ans), true};
}

} // namespace my
