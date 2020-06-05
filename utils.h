#ifndef UTILS_H
#define UTILS_H

#include "ip.h"

#include <vector>
#include <string>

namespace my
{

std::vector<std::string> split(const std::string& str, char d);

my::ip_t splitted_to_ip(const std::vector<std::string>& splitted);

} // namespace my

#endif // UTILS_H
