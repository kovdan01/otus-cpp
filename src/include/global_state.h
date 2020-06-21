#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include "stream_reader.h"
#include "console_writer.h"
#include "file_writer.h"
#include "dummy_command_processor.h"

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
//    my::ConsoleWriter* console_writer() const;
//    my::FileWriter* file_writer() const;
    my::DummyCommandProcessor* command_processor() const;

private:
    GlobalState(std::size_t bulk);

    static inline my::ConsoleWriter m_console_writer;
    static inline my::FileWriter m_file_writer = {"bulk", ".log"};
    static inline my::DummyCommandProcessor m_bulk_command_processor;

    std::size_t m_bulk;
};

} // namespace my

#endif // GLOBAL_STATE_H
