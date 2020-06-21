#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include "console_writer.h"
#include "file_writer.h"
#include "dummy_command_processor.h"
#include "common_command_buffer.h"

namespace my
{

class GlobalState
{
public:
    GlobalState(const GlobalState&) = delete;
    GlobalState& operator=(const GlobalState&) = delete;
    GlobalState(GlobalState&&) = delete;
    GlobalState& operator=(GlobalState&&) = delete;

    static GlobalState* get_instance(std::size_t bulk);

    std::size_t bulk() const;
    DummyCommandProcessor* command_processor();
    CommonCommandBuffer* common_command_buffer();

private:
    GlobalState(std::size_t bulk);

    ConsoleWriter m_console_writer;
    FileWriter m_file_writer = {"bulk", ".log"};
    DummyCommandProcessor m_bulk_command_processor;
    CommonCommandBuffer m_common_command_buffer;

    std::size_t m_bulk;
    //GlobalState* m_instance;
};

} // namespace my

#endif // GLOBAL_STATE_H
