#include "utils/utils.h"

#include <chrono>

namespace my
{

namespace utils
{

std::uint64_t get_current_time()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

} // namespace utils

} // namespace my
