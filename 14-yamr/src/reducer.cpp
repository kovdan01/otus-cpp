#include "reducer.h"

namespace my
{

bool reducer(reducer_iterator begin, reducer_iterator end)
{
    if (begin == end)
        return true;
    for (auto it = std::next(begin); it != end; ++it)
        if (*std::prev(it) == *it)
            return false;
    return true;
}

} // namespace my
