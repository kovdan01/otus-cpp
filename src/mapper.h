#ifndef MAPPER_H
#define MAPPER_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <list>

namespace my
{

using mapper_t = std::function<std::pair<std::list<std::string>, bool> (const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to, std::size_t prefix_size)>;
std::pair<std::list<std::string>, bool> mapper(const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to, std::size_t prefix_size);

} // namespace my

#endif // MAPPER_H
