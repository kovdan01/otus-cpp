#include "reducer.h"

namespace my
{

bool reducer(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end)
{
    if (begin == end)
        return true;
    for (auto it = std::next(begin); it != end; ++it)
        if (*std::prev(it) == *it)
            return false;
    return true;
}

} // namespace my
