#include "bulk_command_processor.h"

#include <sstream>
#include <cassert>

namespace my
{

void BulkCommandProcessor::add_writer(IWriter* writer)
{
    assert(writer != nullptr);
    m_writers.emplace(writer);
}

void BulkCommandProcessor::remove_writer(IWriter* writer)
{
    m_writers.erase(writer);
}

void BulkCommandProcessor::process_data(std::shared_ptr<const std::vector<std::string>> data)
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
}

} // namespace my
