#include "async.h"
#include "console_reader.h"
#include "console_writer.h"
#include "dummy_command_processor.h"
#include "command_storage.h"

#include <unordered_map>
#include <memory>
#include <mutex>
#include <cassert>
#include <string>
#include <sstream>
#include <string_view>

namespace async
{

namespace
{

/**
 * @brief The Controller class
 * Each controller instance matches one handle_t
 */
class Controller
{
public:
    Controller(std::size_t bulk)
        : m_console_reader(bulk)
    {
        m_bulk_command_processor.add_writer(&m_console_writer);
        m_bulk_command_processor.add_writer(&m_file_writer);
        m_command_storage.add_processor(&m_bulk_command_processor);
        m_console_reader.add_storage(&m_command_storage);
    }

    void receive(std::istream& input)
    {
        m_console_reader.set_stream(input);
        m_console_reader.read();
    }

private:
    my::ConsoleWriter m_console_writer;
    my::FileWriter m_file_writer = {"bulk", ".log"};
    my::DummyCommandProcessor m_bulk_command_processor;
    my::CommandStorage m_command_storage;
    my::ConsoleReader m_console_reader;
};

/**
 * Helper custom deleter for unique_ptr
 * Used to search in hash table of unique_ptr's
 */
template<class T>
struct maybe_deleter
{
public:
    explicit maybe_deleter(bool need_delete = true)
        : m_delete(need_delete)
    {
    }

    void operator()(T* p) const
    {
        if (m_delete)
            delete p;
    }

private:
    bool m_delete;
};


template<class T>
using set_unique_ptr = std::unique_ptr<T, maybe_deleter<T>>;

set_unique_ptr<Controller> make_find_ptr(handle_t raw)
{
    return set_unique_ptr<Controller>(reinterpret_cast<Controller*>(raw), maybe_deleter<Controller>(false));
}

/**
 * @brief g_connections
 * key - unique pointer to Controller
 * value - string buffer (string part from last new line symbol to the end)
 */
static std::unordered_map<set_unique_ptr<Controller>, std::string> g_connections;
static std::mutex g_mutex;

} // namespace


handle_t connect(std::size_t bulk)
{
    std::lock_guard lock(g_mutex);

    auto [it, b] = g_connections.emplace(set_unique_ptr<Controller>(new Controller(bulk)), std::string{});
    assert(b);

    return it->first.get();
}


void receive(handle_t handle, const char* data, std::size_t size)
{
    std::lock_guard lock(g_mutex);
    auto it = g_connections.find(make_find_ptr(handle));
    if (it == g_connections.end())
        return;

    std::string_view view(data, size);
    std::size_t pos = view.rfind('\n');
    if (pos == std::string_view::npos)
    {
        it->second += std::string(data, size);
        return;
    }

    std::string str(data, pos + 1);
    std::istringstream input(it->second + str);
    it->second = std::string(data + pos + 1, size - pos - 1);
    it->first->receive(input);
}


void disconnect(handle_t handle)
{
    std::lock_guard lock(g_mutex);
    g_connections.erase(make_find_ptr(handle));
}

} // namespace async
