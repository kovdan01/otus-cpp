#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "interfaces/processor.h"
#include "interfaces/writer.h"
#include "file_writer.h"

#include <unordered_set>

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
    void add_writer(const IWriter* writer) override;

    /**
     * @brief Adds special FileWriter logger for special different behavior
     * @param writer[in] Pointer to logger to be added
     */
    void add_writer(const FileWriter* writer);
    void remove_writer(const IWriter* writer) override;

    void process_data(std::shared_ptr<const std::vector<std::string>> data, std::uint64_t first_command_time) override;

    virtual ~DummyCommandProcessor() = default;

private:
    std::unordered_set<const IWriter*> m_writers;
    std::unordered_set<const FileWriter*> m_file_writers;
};

} // namespace my

#endif // COMMAND_PROCESSOR_H
