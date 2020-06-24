#ifndef RAW_DATA_HANDLER_H
#define RAW_DATA_HANDLER_H

#include "implementation/stream_reader.h"
#include "implementation/console_writer.h"
#include "implementation/file_writer.h"
#include "implementation/dummy_command_processor.h"

namespace my
{

/**
 * @brief The RawDataHandler class
 * Processes data received from network.
 * If some data can be processed, passes it to StreamReader
 */
class RawDataHandler
{
public:
    RawDataHandler(std::size_t bulk);

    RawDataHandler(const RawDataHandler&) = delete;
    RawDataHandler& operator=(const RawDataHandler&) = delete;
    RawDataHandler(RawDataHandler&&) = delete;
    RawDataHandler& operator=(RawDataHandler&&) = delete;

    /**
     * @brief Add new portion of data from network
     * @param data[in] Pointer to the beginning of data buffer
     * @param size[in] Buffer size
     */
    void receive(const char* data, std::size_t size);

private:
    my::StreamReader m_stream_reader;
    std::string m_temp_str;
};

} // namespace my

#endif // RAW_DATA_HANDLER_H
