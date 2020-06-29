#include "console_writer.h"

#include <iostream>

namespace my
{

void ConsoleWriter::write(const std::string& str) const
{
    std::cout << str;
}

} // namespace my
