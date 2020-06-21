#include "implementation/console_writer.h"

#include <iostream>

namespace my
{

void ConsoleWriter::write(const std::string& str) const
{
    std::lock_guard lock(m_mutex);
    std::cout << str;
    std::cout.flush();
}

} // namespace my
