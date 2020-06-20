#include "console_writer.h"

#include <iostream>

namespace my
{

void ConsoleWriter::write(const std::string& str) const
{
    std::cout << str;
    std::cout.flush();
}

} // namespace my
