#ifndef COMMAND_STORAGE_H
#define COMMAND_STORAGE_H

#include "interfaces/storage.h"
#include "interfaces/processor.h"

#include <vector>
#include <memory>
#include <string>
#include <unordered_set>
#include <mutex>

namespace my
{

/**
 * @brief The CommandStorage class
 * Represents a storage for commands, that at first accumulates a block
 * of data and then gives it to processors-subscribers
 */
class CommandStorage : public IStorage
{
public:
    CommandStorage();

    void add_processor(IProcessor* processor) override;
    void remove_processor(IProcessor* processor) override;

    void new_input(const std::string& input) override;
    void end_input() override;

    void set_block_size(std::size_t block_size);

    void pass_through(std::shared_ptr<std::vector<std::string>> commands);

    virtual ~CommandStorage() = default;

private:
    void end_input_non_blocking();
    void open_brace();
    void close_brace();
    unsigned int m_braces_level = 0;
    std::size_t m_block_size = 1;
    std::size_t m_lines_accumulated = 0;

    std::shared_ptr<std::vector<std::string>> m_commands;
    std::unordered_set<IProcessor*> m_processors;
    std::uint64_t m_first_command_time = 0;
    std::mutex m_mutex;
};

} // namespace my

#endif // COMMAND_STORAGE_H
