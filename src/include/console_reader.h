#ifndef CONSOLE_READER_H
#define CONSOLE_READER_H

#include "interfaces/reader.h"
#include "command_storage.h"
#include "common_command_buffer.h"

#include <unordered_set>
#include <string>
#include <iostream>
#include <mutex>

namespace my
{

/**
 * @brief The ConsoleReader class
 * Represents a line-by-line reader from `std::istream` (stdin by default)
 */
class ConsoleReader : public IReader
{
public:
    ConsoleReader(std::size_t block_size);

    void set_processor(IProcessor* processor);
//    void add_processor(IProcessor* processor);
//    void remove_processor(IProcessor* processor);

    void read() override;
    void set_stream(std::shared_ptr<std::istream> input);

    virtual ~ConsoleReader();

private:
    void init();

    void open_brace();
    void close_brace();
    void new_input(const std::string& input);

    void end_input_non_blocking();
    //void end_input();

//    void notify_end_input();
//    void notify_new_line(const std::string& line);

    std::shared_ptr<std::vector<std::string>> m_this_commands;
    //static inline std::shared_ptr<std::vector<std::string>> m_shared_commands;
    CommonCommandBuffer* m_common = nullptr;
    std::mutex m_mutex;

    //std::unordered_set<IProcessor*> m_processors;
    IProcessor* m_processor;

    std::uint64_t m_first_command_time;

    std::unordered_set<CommandStorage*> m_storages;
    std::shared_ptr<std::istream> m_input;
    std::size_t m_block_size;
    //std::size_t m_lines_accumulated = 0;
    unsigned int m_braces_level = 0;
};

} // namespace my

#endif // CONSOLE_READER_H
