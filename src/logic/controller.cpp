#include "controller.h"
#include "global_state.h"
#include "stream_reader.h"

#include <iostream>
#include <string>
#include <sstream>
#include <mutex>

namespace my
{

Controller::Controller(std::size_t bulk)
    : m_console_reader(GlobalState::get_instance(bulk))
{
    m_console_reader.set_processor(GlobalState::get_instance(bulk)->command_processor());
}

void Controller::receive(const char* data, std::size_t size)
{
    std::string str(data, size);

    std::size_t pos = str.rfind('\n');
    if (pos == std::string::npos)
    {
        m_temp_str += str;
        return;
    }

    std::shared_ptr<std::istream> input = std::make_shared<std::istringstream>(m_temp_str + str.substr(0, pos));
    m_temp_str = std::string(str.data() + pos + 1, str.size() - pos - 1);

    m_console_reader.set_stream(std::move(input));
    m_console_reader.read();
}

} // namespace my
