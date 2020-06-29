#ifndef REDUCER_H
#define REDUCER_H

#include <functional>
#include <string>
#include <vector>

namespace my
{

using reducer_iterator = std::vector<std::string>::const_iterator;
using reducer_t = std::function<bool (reducer_iterator begin, reducer_iterator end)>;

/**
 * @brief reducer Checks if there are duplicates in a given sorted range
 * @param begin,end Iterators to the sorted range
 * @return Returns true if there are no duplicates in the given range, false otherwise
 */
bool reducer(reducer_iterator begin, reducer_iterator end);

} // namespace my

#endif // REDUCER_H
