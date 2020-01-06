#pragma once

#include <string>
#include <cstddef>

struct Version
{
    std::uint32_t major, minor, patch;
};

Version version();
std::string message();
