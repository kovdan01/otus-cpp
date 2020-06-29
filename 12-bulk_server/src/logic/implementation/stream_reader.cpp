#include "implementation/stream_reader.h"
#include "implementation/global_state.h"
#include "utils/utils.h"
#include "utils/thread_pool.h"

#include <cassert>
#include <iostream>

namespace my
{

StreamReader::StreamReader(GlobalState* global_state)
    : m_this_commands(std::make_shared<std::vector<std::string>>())
    , m_common(global_state->common_command_buffer())
    , m_processor(global_state->command_processor())
    , m_bulk(global_state->bulk())
{
}

void StreamReader::add_command(const std::string& input)
{
    if (m_braces_level == 0)
    {
        m_common->add_command(input);
    }
    else
    {
        if (m_this_commands->empty())
            m_first_command_time = utils::get_current_time();
        m_this_commands->emplace_back(input);
    }
}

void StreamReader::end_input_non_blocking()
{

}

void StreamReader::read()
{
    if (m_input == nullptr)
        return;

    std::string line;
    while (std::getline(*m_input, line))
    {
        if (line == "{")
            open_brace();
        else if (line == "}")
            close_brace();
        else
            add_command(line);
    }
}

void StreamReader::set_stream(std::shared_ptr<std::istream> input)
{
    m_input = std::move(input);
}

void StreamReader::open_brace()
{
    ++m_braces_level;
}

void StreamReader::close_brace()
{
    if (m_braces_level == 0)
        throw std::runtime_error("Too many closing braces don't match opening braces");

    --m_braces_level;
    if (m_braces_level != 0)
        return;

    m_common->send_commands_to_processor();

    std::shared_ptr<std::vector<std::string>> old_commands = m_this_commands;
    std::uint64_t old_time = m_first_command_time;
    IProcessor* processor = m_processor;
    m_this_commands = std::make_shared<std::vector<std::string>>();

    auto job = [processor, old_commands, old_time]()
    {
        processor->process_data(old_commands, old_time);
    };
    progschj::ThreadPool::get_instance()->enqueue(job);
}

} // namespace my
