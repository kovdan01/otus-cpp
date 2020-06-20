#ifndef REDUCER_H
#define REDUCER_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>

namespace my
{

using reducer_iterator = std::vector<std::string>::const_iterator;
using reducer_t = std::function<bool (reducer_iterator begin, reducer_iterator end)>;
bool reducer(reducer_iterator begin, reducer_iterator end);

} // namespace my

#endif // REDUCER_H
