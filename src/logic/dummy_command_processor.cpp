#include "dummy_command_processor.h"
#include "dummy_command.h"
#include "stats.h"

#include <sstream>
#include <cassert>

#include <iostream>

namespace my
{

DummyCommandProcessor::DummyCommandProcessor() = default;

DummyCommandProcessor::~DummyCommandProcessor()
{
}

std::map<std::thread::id, DummyCommandProcessor::Stats> DummyCommandProcessor::console_stats() const
{
    return m_console_stats;
}

std::map<std::thread::id, DummyCommandProcessor::Stats> DummyCommandProcessor::file_stats() const
{
    return m_file_writer_stats;
}

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

    ++MainStats::get_instance()->blocks_count;

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

    std::shared_ptr<std::string> str = std::make_shared<std::string>(output.str());
    std::size_t block_size = data->size();
    auto log_counters = [block_size]()
    {
        auto& stats = LogStats::get_instance()->stats;
        std::thread::id id = std::this_thread::get_id();
        ++stats[id].blocks_count;
        stats[id].commands_count += block_size;
    };

    for (const IWriter* writer : m_writers)
    {
        auto job = [writer, str, log_counters]()
        {
            writer->write(*str);
            log_counters();
        };
        m_log_job.enqueue(job);
    }

    for (const FileWriter* writer : m_file_writers)
    {
        auto job = [writer, str, first_command_time, log_counters]()
        {
            writer->write(*str, first_command_time);
            log_counters();
        };
        m_file_writer_jobs.enqueue(job);
    }
}

} // namespace my
