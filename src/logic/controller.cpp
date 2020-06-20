#include "controller.h"
#include "console_reader.h"

#include <iostream>
#include <string>
#include <sstream>

namespace my
{

ControllerSingletone::ControllerSingletone(std::size_t bulk)
    : m_console_reader(bulk)
    , m_threads(4)
{
    m_bulk_command_processor.add_writer(&m_console_writer);
    m_bulk_command_processor.add_writer(&m_file_writer);
    m_command_storage.add_processor(&m_bulk_command_processor);
    m_console_reader.add_storage(&m_command_storage);
}

ControllerSingletone* ControllerSingletone::get_instance(std::size_t bulk)
{
    static ControllerSingletone instance(bulk);
    return &instance;
}

void ControllerSingletone::receive(const char* data, std::size_t size)
{
    std::shared_ptr<std::string> str = std::make_unique<std::string>(data, size);
    auto job = [this, str]()
    {
        std::size_t pos = str->rfind('\n');
        if (pos == std::string::npos)
        {
            std::lock_guard lock(m_mutex);
            m_temp_str += *str;
            return;
        }

        std::lock_guard lock(m_mutex);
        m_input = std::make_shared<std::istringstream>(m_temp_str + *str);
        m_temp_str = std::string(str->data() + pos + 1, str->size() - pos - 1);

        m_console_reader.set_stream(*m_input);
        m_console_reader.read();
    };
    m_threads.enqueue(job);
}

} // namespace my
