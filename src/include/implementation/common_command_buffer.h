#ifndef COMMON_COMMAND_BUFFER_H
#define COMMON_COMMAND_BUFFER_H

#include "interfaces/processor.h"

#include <vector>
#include <memory>
#include <string>

namespace my
{

class CommonCommandBuffer
{
public:
    CommonCommandBuffer(std::size_t bulk, IProcessor* processor);
    ~CommonCommandBuffer();

    void add_command(const std::string& command);
    void send_commands_to_processor();

private:
    std::size_t m_bulk;
    IProcessor* m_processor;

    std::uint64_t m_first_command_time = 0;
    std::shared_ptr<std::vector<std::string>> m_commands;
};

} // namespace my

#endif // COMMON_COMMAND_BUFFER_H
