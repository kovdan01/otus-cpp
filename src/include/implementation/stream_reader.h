#ifndef STREAM_READER_H
#define STREAM_READER_H

#include "interfaces/reader.h"
#include "common_command_buffer.h"
#include "global_state.h"

#include <string>
#include <istream>

namespace my
{

/**
 * @brief The StreamReader class
 * Represent a commands reader from `std::istream`, that supports
 * asynchronous work. Commands inside { } are stored in a buffer
 * inside the instance of this class, "global" commans are stored in
 * CommonCommandBuffer instance, that is unique for the whole program
 */
class StreamReader : public IReader
{
public:
    StreamReader(GlobalState* global_state);

    void read() override;

    /**
     * @brief Set input stream
     * @param input[in] Shared pointer to the input stream
     */
    void set_stream(std::shared_ptr<std::istream> input);

    virtual ~StreamReader() = default;

private:
    void open_brace();
    void close_brace();
    void add_command(const std::string& input);

    void end_input_non_blocking();

    std::shared_ptr<std::vector<std::string>> m_this_commands;

    CommonCommandBuffer* m_common;
    IProcessor* m_processor;

    std::uint64_t m_first_command_time = 0;

    std::shared_ptr<std::istream> m_input;
    std::size_t m_bulk;
    unsigned int m_braces_level = 0;
};

} // namespace my

#endif // STREAM_READER_H
