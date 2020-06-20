#ifndef SPLITTER_H
#define SPLITTER_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>

namespace my
{

using splitter_return_t = std::vector<std::pair<std::ifstream::pos_type, std::ifstream::pos_type>>;
using splitter_t = std::function<splitter_return_t (const std::string& filename, std::size_t count)>;
splitter_return_t splitter (const std::string& filename, std::size_t count);

} // namespace my

#endif // SPLITTER_H
