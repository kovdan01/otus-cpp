#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "implementation/stream_reader.h"
#include "implementation/console_writer.h"
#include "implementation/file_writer.h"
#include "implementation/dummy_command_processor.h"

namespace my
{

/**
 * @brief The Controller class
 * Handles queries
 */
class Controller
{
public:
    friend class Session;

    Controller(std::size_t bulk);

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

    void receive(const char* data, std::size_t size);

private:
    my::StreamReader m_stream_reader;
    std::string m_temp_str;
};

} // namespace my

#endif // CONTROLLER_H
