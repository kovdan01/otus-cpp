#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "console_reader.h"
#include "console_writer.h"
#include "dummy_command_processor.h"
#include "command_storage.h"
#include "thread_pool.h"
#include "network_session.h"

#include <mutex>
#include <set>

namespace my
{

/**
 * @brief The ControllerSingletone class
 * Handles queries
 */
class ControllerSingletone
{
public:
    ControllerSingletone(const ControllerSingletone&) = delete;
    ControllerSingletone& operator=(const ControllerSingletone&) = delete;
    ControllerSingletone(ControllerSingletone&&) = delete;
    ControllerSingletone& operator=(ControllerSingletone&&) = delete;

    static ControllerSingletone* get_instance(std::size_t bulk);

    void receive(const char* data, std::size_t size, Session* session);

private:
    ControllerSingletone(std::size_t bulk);

    my::ConsoleWriter m_console_writer;
    my::FileWriter m_file_writer = {"bulk", ".log"};
    my::DummyCommandProcessor m_bulk_command_processor;
    my::CommandStorage m_command_storage;
    struct Reader
    {
        my::ConsoleReader reader;
        std::shared_ptr<std::istringstream> input;
        std::string temp_str;
        std::mutex mutex;
    };

    std::set<Session*, Reader> m_console_readers;

//    std::shared_ptr<std::istringstream> m_input;
//    std::string m_temp_str;

    std::mutex m_mutex;
    //std::set<Session*, std::mutex> m_reader_mutexes;
    progschj::ThreadPool m_threads;
};

} // namespace my

#endif // CONTROLLER_H
