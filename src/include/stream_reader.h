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

class StreamReader : public IReader
{
public:
    StreamReader(std::size_t block_size);

    void set_processor(IProcessor* processor);

    void read() override;
    void set_stream(std::shared_ptr<std::istream> input);

    virtual ~StreamReader() = default;

private:

    void open_brace();
    void close_brace();
    void new_input(const std::string& input);

    void end_input_non_blocking();

    std::shared_ptr<std::vector<std::string>> m_this_commands;
    CommonCommandBuffer* m_common = nullptr;

    IProcessor* m_processor;

    std::uint64_t m_first_command_time;

    std::unordered_set<CommandStorage*> m_storages;
    std::shared_ptr<std::istream> m_input;
    std::size_t m_block_size;
    unsigned int m_braces_level = 0;
};

} // namespace my

#endif // CONSOLE_READER_H
