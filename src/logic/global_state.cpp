#include "global_state.h"

namespace my
{

GlobalState::GlobalState(std::size_t bulk)
    : m_bulk(bulk)
{
    m_bulk_command_processor.add_writer(&m_console_writer);
    m_bulk_command_processor.add_writer(&m_file_writer);
}

GlobalState* GlobalState::get_instance(std::size_t bulk)
{
    static GlobalState instance(bulk);
    return &instance;
}

std::size_t GlobalState::bulk() const
{
    return m_bulk;
}

my::DummyCommandProcessor* GlobalState::command_processor() const
{
    return &m_bulk_command_processor;
}

} // namespace my
