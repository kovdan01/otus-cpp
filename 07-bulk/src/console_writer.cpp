#include "console_writer.h"

#include <iostream>

namespace my
{

void ConsoleWriter::write(const std::string& str)
{
    std::cout << str;
}

} // namespace my
