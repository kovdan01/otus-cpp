#include "console_reader.h"
#include "stats.h"

#include <cassert>
#include <iostream>

namespace my
{

ConsoleReader::ConsoleReader(std::size_t block_size, std::istream& input)
    : m_input(&input)
    , m_block_size(block_size)
{
}

void ConsoleReader::add_storage(IStorage* storage)
{
    assert(storage != nullptr);
    m_storages.emplace(storage);
}

void ConsoleReader::remove_storage(IStorage* storage)
{
    m_storages.erase(storage);
}

void ConsoleReader::read()
{
    std::string line;
    MainStats* stats = MainStats::get_instance();
    while (std::getline(*m_input, line))
    {
        ++stats->lines_count;
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
            ++stats->commands_count;
            ++m_lines_accumulated;
            notify_new_line(line);
            if (m_lines_accumulated == m_block_size && m_braces_level == 0)
            {
                m_lines_accumulated = 0;
                notify_end_input();
            }
        }
    }
}

ConsoleReader::~ConsoleReader()
{
    if (m_braces_level == 0)
        notify_end_input();
}

void ConsoleReader::set_stream(std::istream& input)
{
    m_input = &input;
}

void ConsoleReader::open_brace()
{
    m_lines_accumulated = 0;
    ++m_braces_level;
    if (m_braces_level == 1)
        notify_end_input();
}

void ConsoleReader::close_brace()
{
    if (m_braces_level == 0)
        throw std::runtime_error("Too many closing braces don't match opening braces");
    m_lines_accumulated = 0;
    --m_braces_level;
    if (m_braces_level == 0)
        notify_end_input();
}

void ConsoleReader::notify_end_input()
{
    for (IStorage* storage : m_storages)
        storage->end_input();
}

void ConsoleReader::notify_new_line(const std::string& line)
{
    for (IStorage* storage : m_storages)
        storage->new_input(line);
}

} // namespace my
