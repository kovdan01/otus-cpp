#include "command_storage.h"

#include <cassert>
#include <chrono>

#include <iostream>

namespace my
{

CommandStorage::CommandStorage()
    : m_commands(std::make_shared<std::vector<std::string>>())
{
}

void CommandStorage::add_processor(IProcessor* processor)
{
    assert(processor != nullptr);
    m_processors.emplace(processor);
}

void CommandStorage::remove_processor(IProcessor* processor)
{
    m_processors.erase(processor);
}

void CommandStorage::new_input(const std::string& input)
{
    std::lock_guard lock(m_mutex);
    if (input == "{")
    {
        open_brace();
    }
    else if (input == "}")
    {
        close_brace();
    }
    else
    {
        if (m_commands->empty())
        {
            using namespace std::chrono;
            m_first_command_time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        }
        m_commands->emplace_back(input);
        ++m_lines_accumulated;
        //std::cout << "LINES: " << m_lines_accumulated << std::endl;
        if (m_lines_accumulated == m_block_size && m_braces_level == 0)
        {
            m_lines_accumulated = 0;
            end_input_non_blocking();
        }
    }
}

void CommandStorage::open_brace()
{
    m_lines_accumulated = 0;
    ++m_braces_level;
    if (m_braces_level == 1)
        end_input_non_blocking();
}

void CommandStorage::close_brace()
{
    if (m_braces_level == 0)
        throw std::runtime_error("Too many closing braces don't match opening braces");
    m_lines_accumulated = 0;
    --m_braces_level;
    if (m_braces_level == 0)
        end_input_non_blocking();
}

void CommandStorage::set_block_size(std::size_t block_size)
{
    m_block_size = block_size;
}

void CommandStorage::end_input()
{
    std::lock_guard lock(m_mutex);
    end_input_non_blocking();
}

void CommandStorage::end_input_non_blocking()
{
    for (IProcessor* processor : m_processors)
        processor->process_data(m_commands, m_first_command_time);

    m_commands = std::make_shared<std::vector<std::string>>();
    m_first_command_time = 0;
}

//void CommandStorage::pass_through(std::shared_ptr<std::vector<std::string>> commands)
//{
//    for (IProcessor* processor : m_processors)
//        processor->process_data(commands, m_first_command_time);
//}

} // namespace my
