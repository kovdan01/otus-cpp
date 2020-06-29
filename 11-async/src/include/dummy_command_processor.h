#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "interfaces/processor.h"
#include "interfaces/writer.h"
#include "file_writer.h"
#include "thread_pool.h"

#include <unordered_set>
#include <mutex>
#include <thread>
#include <map>

namespace my
{

/**
 * @brief The DummyCommandProcessor class
 * Represents a dummy command processor, that executes commands,
 * prints command names to given loggers and logs
 * command blocks to different files using FileWriter loggers
 */
class DummyCommandProcessor : public IProcessor
{
public:
    DummyCommandProcessor();

    void add_writer(const IWriter* writer) override;

    /**
     * @brief Adds special FileWriter logger for special different behavior
     * @param writer[in] Pointer to logger to be added
     */
    void add_writer(const FileWriter* writer);
    void remove_writer(const IWriter* writer) override;

    void process_data(std::shared_ptr<const std::vector<std::string>> data, std::uint64_t first_command_time) override;

    virtual ~DummyCommandProcessor();

    struct Stats
    {
        std::size_t blocks_count = 0;
        std::size_t commands_count = 0;
    };

    std::map<std::thread::id, Stats> console_stats() const;
    std::map<std::thread::id, Stats> file_stats() const;

private:
    std::unordered_set<const IWriter*> m_writers;
    std::unordered_set<const FileWriter*> m_file_writers;

    progschj::ThreadPool m_file_writer_jobs = {2};
    progschj::ThreadPool m_log_job = {1};

    std::map<std::thread::id, Stats> m_file_writer_stats;
    std::map<std::thread::id, Stats> m_console_stats;
    std::mutex m_console_stats_lock;
    std::mutex m_file_stats_lock;
};

} // namespace my

#endif // COMMAND_PROCESSOR_H
