#include "implementation/dummy_command_processor.h"
#include "implementation/dummy_command.h"

#include <sstream>
#include <cassert>

namespace my
{

void DummyCommandProcessor::add_writer(const IWriter* writer)
{
    assert(writer != nullptr);
    m_writers.emplace(writer);
}

void DummyCommandProcessor::add_writer(const FileWriter* writer)
{
    assert(writer != nullptr);
    m_file_writers.emplace(writer);
}

void DummyCommandProcessor::remove_writer(const IWriter* writer)
{
    m_writers.erase(writer);
}

void DummyCommandProcessor::process_data(std::shared_ptr<const std::vector<std::string>> data, std::uint64_t first_command_time)
{
    if (data->empty())
        return;

    std::ostringstream output;
    output << "bulk: ";
    for (auto it = data->begin(); it != data->end(); ++it)
    {
        const std::string& command_name = *it;
        DummyCommand command(command_name);
        command.execute();
        if (it == data->begin())
            output << command_name;
        else
            output << ", " << command_name;
    }
    output << '\n';

    std::string str = output.str();
    for (const IWriter* writer : m_writers)
        writer->write(str);
    for (const FileWriter* writer : m_file_writers)
        writer->write(str, first_command_time);
}

} // namespace my

