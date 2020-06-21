#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "console_reader.h"
#include "console_writer.h"
#include "dummy_command_processor.h"
#include "command_storage.h"
#include "thread_pool.h"

#include <mutex>
#include <sstream>

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

    //static Controller* get_instance(std::size_t bulk);

    void receive(const char* data, std::size_t size);

private:
    void init(std::size_t bulk);
    static inline my::ConsoleWriter m_console_writer;
    static inline my::FileWriter m_file_writer = {"bulk", ".log"};
    static inline my::DummyCommandProcessor m_bulk_command_processor;
    //static inline my::CommandStorage m_command_storage;
    //static std::size_t m_bulk;
    my::ConsoleReader m_console_reader;

    //std::istringstream m_input;
    std::string m_temp_str;

    std::mutex m_mutex;
    progschj::ThreadPool m_threads;
};

} // namespace my

#endif // CONTROLLER_H
