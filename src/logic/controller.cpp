#include "controller.h"
#include "console_reader.h"

#include <iostream>
#include <string>
#include <sstream>
#include <mutex>

namespace my
{

Controller::Controller(std::size_t bulk)
    : m_console_reader(bulk)
    , m_threads(4)
{
    init(bulk);
    m_console_reader.set_processor(&m_bulk_command_processor);
}

void Controller::init(std::size_t /*bulk*/)
{
    static bool is_init = false;
    if (!is_init)
    {
        m_bulk_command_processor.add_writer(&m_console_writer);
        m_bulk_command_processor.add_writer(&m_file_writer);
//        m_command_storage.add_processor(&m_bulk_command_processor);
//        m_command_storage.set_block_size(bulk);
        is_init = true;
    }
}

void Controller::receive(const char* data, std::size_t size)
{
    std::string str(data, size);
    std::lock_guard lock(m_mutex);
    std::size_t pos = str.rfind('\n');
    if (pos == std::string::npos)
    {
        m_temp_str += str;
        return;
    }

    std::shared_ptr<std::istream> input = std::make_shared<std::istringstream>(m_temp_str + str.substr(0, pos));
    m_temp_str = std::string(str.data() + pos + 1, str.size() - pos - 1);

    auto job = [this, input]()
    {
        std::lock_guard lock(m_mutex);
        m_console_reader.set_stream(std::move(input));
        m_console_reader.read();
    };
    m_threads.enqueue(job);
}

} // namespace my
