#include "console_reader.h"

#include <cassert>
#include <iostream>

namespace my
{

ConsoleReader::ConsoleReader(std::size_t block_size)
    : m_this_commands(std::make_shared<std::vector<std::string>>())
    , m_block_size(block_size)
{
    //init();
}

void ConsoleReader::init()
{
//    static bool init = false;
//    if (!init)
//    {
//        m_shared_commands = std::make_shared<std::vector<std::string>>();
//        init = true;
//    }
}

void ConsoleReader::new_input(const std::string& input)
{
    //std::lock_guard lock(m_mutex);
    //++m_lines_accumulated;
//    if (m_shared_commands->empty())
//    {
//        using namespace std::chrono;
//        m_first_command_time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
//    }
    if (m_braces_level == 0)
    {
        m_common->add_command(input);
    }
    else
    {
        if (m_this_commands->empty())
        {
            using namespace std::chrono;
            m_first_command_time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        }
        m_this_commands->emplace_back(input);
    }
//    m_shared_commands->emplace_back(input);
//    if (m_lines_accumulated == m_block_size && m_braces_level == 0)
//    {
//        m_lines_accumulated = 0;
//        end_input_non_blocking();
//    }
}


//void ConsoleReader::add_processor(IProcessor* processor)
//{
//    assert(processor != nullptr);
//    m_processors.emplace(processor);
//}

void ConsoleReader::set_processor(IProcessor* processor)
{
    assert(processor != nullptr);
    m_processor = processor;
    m_common = CommonCommandBuffer::get_instance(m_block_size, m_processor);
}

//void ConsoleReader::remove_processor(IProcessor* processor)
//{
//    m_processors.erase(processor);
//}


//void ConsoleReader::end_input()
//{
//    std::lock_guard lock(m_mutex);
//    end_input_non_blocking();
//}

void ConsoleReader::end_input_non_blocking()
{
//    for (IProcessor* processor : m_processors)
//        processor->process_data(m_shared_commands, m_first_command_time);
    if (!m_this_commands->empty())
        m_processor->process_data(m_this_commands, m_first_command_time);

    m_this_commands = std::make_shared<std::vector<std::string>>();
    m_first_command_time = 0;
}


void ConsoleReader::read()
{
    std::lock_guard lock(m_mutex);
    std::string line;
    while (std::getline(*m_input, line))
    {
        if (line == "{")
        {
            open_brace();
        }
        else if (line == "}")
        {
            close_brace();
        }
        else
        {
            new_input(line);
        }
    }
}

ConsoleReader::~ConsoleReader()
{
//    std::lock_guard lock(m_mutex);
//    if (m_braces_level == 0)
//        end_input_non_blocking();
}

void ConsoleReader::set_stream(std::shared_ptr<std::istream> input)
{
    std::lock_guard lock(m_mutex);
    m_input = std::move(input);
}

void ConsoleReader::open_brace()
{
    //std::lock_guard lock(m_mutex);
    //m_lines_accumulated = 0;
    ++m_braces_level;
//    if (m_braces_level == 1)
//    {
//        end_input_non_blocking();
//    }
}

void ConsoleReader::close_brace()
{
    //std::lock_guard lock(m_mutex);
    if (m_braces_level == 0)
        throw std::runtime_error("Too many closing braces don't match opening braces");
    //m_lines_accumulated = 0;
    --m_braces_level;
    if (m_braces_level == 0)
    {
        m_common->to_processor();
        end_input_non_blocking();
    }
}

//void ConsoleReader::notify_end_input()
//{
//    for (IStorage* storage : m_storages)
//        storage->end_input();
//}

//void ConsoleReader::notify_new_line(const std::string& line)
//{
//    for (IStorage* storage : m_storages)
//        storage->new_input(line);
//}

} // namespace my
