#include "command_storage.h"

#include <cassert>
#include <chrono>

namespace my
{

CommandStorage::CommandStorage()
    : m_commands(std::make_shared<std::vector<std::string>>())
{
}

void CommandStorage::add_processor(IProcessor* processor)
{
    assert(processor != nullptr);
    m_processors.emplace(processor);
}

void CommandStorage::remove_processor(IProcessor* processor)
{
    m_processors.erase(processor);
}

void CommandStorage::new_input(const std::string& input)
{
    if (m_commands->empty())
    {
        using namespace std::chrono;
        m_first_command_time = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
    }
    m_commands->emplace_back(input);
}

void CommandStorage::end_input()
{
    for (IProcessor* processor : m_processors)
        processor->process_data(m_commands, m_first_command_time);

    m_commands = std::make_shared<std::vector<std::string>>();
    m_first_command_time = 0;
}

} // namespace my
