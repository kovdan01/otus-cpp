#include "bulk_command_processor.h"
#include "bulk_command.h"

#include <sstream>
#include <cassert>

namespace my
{

void BulkCommandProcessor::add_writer(IWriter* writer)
{
    assert(writer != nullptr);
    m_writers.emplace(writer);
}

void BulkCommandProcessor::add_writer(FileWriter* writer)
{
    assert(writer != nullptr);
    m_file_writers.emplace(writer);
}

void BulkCommandProcessor::remove_writer(IWriter* writer)
{
    m_writers.erase(writer);
}

void BulkCommandProcessor::process_data(std::shared_ptr<const std::vector<std::string>> data, std::uint64_t first_command_time)
{
    if (data->empty())
        return;

    std::ostringstream output;
    output << "bulk: ";
    for (auto it = data->begin(); it != data->end(); ++it)
    {
        const std::string& command_name = *it;
        BulkCommand command(command_name);
        command.execute();
        if (it == data->begin())
            output << command_name;
        else
            output << ", " << command_name;
    }
    output << '\n';

    std::string str = output.str();
    for (IWriter* writer : m_writers)
        writer->write(str);
    for (FileWriter* writer : m_file_writers)
        writer->write(str, first_command_time);
}

} // namespace my
