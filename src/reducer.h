#ifndef REDUCER_H
#define REDUCER_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>

namespace my
{

/**
 * True if no duplicates
 */
using reducer_t = std::function<bool (std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end)>;
bool reducer(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end);

} // namespace my

#endif // REDUCER_H
