#ifndef MAPPER_H
#define MAPPER_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>

namespace my
{

struct PrefixException : public std::exception
{
};

using mapper_t = std::function<std::list<std::string> (const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to, std::size_t prefix_size)>;
std::list<std::string> mapper(const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to, std::size_t prefix_size);

} // namespace my

#endif // MAPPER_H
