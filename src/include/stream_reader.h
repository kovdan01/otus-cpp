#ifndef STREAM_READER_H
#define STREAM_READER_H

#include "interfaces/reader.h"
#include "common_command_buffer.h"
#include "global_state.h"

#include <string>
#include <istream>

namespace my
{

class GlobalState;
class CommonCommandBuffer;

class StreamReader : public IReader
{
public:
    StreamReader(GlobalState* global_state);

    void set_processor(IProcessor* processor);

    void read() override;
    void set_stream(std::shared_ptr<std::istream> input);

    virtual ~StreamReader() = default;

private:

    void open_brace();
    void close_brace();
    void new_input(const std::string& input);

    void end_input_non_blocking();

    //GlobalState* m_global_state;

    std::shared_ptr<std::vector<std::string>> m_this_commands;
    CommonCommandBuffer* m_common = nullptr;

    IProcessor* m_processor;

    std::uint64_t m_first_command_time;

    std::shared_ptr<std::istream> m_input;
    std::size_t m_bulk;
    unsigned int m_braces_level = 0;
};

} // namespace my

#endif // STREAM_READER_H
