#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stream_reader.h"
#include "console_writer.h"
#include "file_writer.h"
#include "dummy_command_processor.h"

namespace my
{

/**
 * @brief The ControllerSingletone class
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
//    static inline my::ConsoleWriter m_console_writer;
//    static inline my::FileWriter m_file_writer = {"bulk", ".log"};
//    static inline my::DummyCommandProcessor m_bulk_command_processor;
    my::StreamReader m_console_reader;

    std::string m_temp_str;
};

} // namespace my

#endif // CONTROLLER_H
