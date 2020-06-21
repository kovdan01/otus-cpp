#include "implementation/common_command_buffer.h"
#include "utils/utils.h"
#include "utils/thread_pool.h"

namespace my
{

CommonCommandBuffer::CommonCommandBuffer(std::size_t bulk, IProcessor* processor)
    : m_bulk(bulk)
    , m_processor(processor)
    , m_commands(std::make_shared<std::vector<std::string>>())
{
}

CommonCommandBuffer::~CommonCommandBuffer()
{
    if (!m_commands->empty())
        m_processor->process_data(m_commands, m_first_command_time);
}

void CommonCommandBuffer::add_command(const std::string& command)
{
    m_commands->emplace_back(command);
    if (m_commands->size() == 1)
        m_first_command_time = utils::get_current_time();
    if (m_commands->size() == m_bulk)
        send_commands_to_processor();
}

void CommonCommandBuffer::send_commands_to_processor()
{
    std::shared_ptr<std::vector<std::string>> old_commands = m_commands;
    std::uint64_t old_time = m_first_command_time;
    IProcessor* processor = m_processor;

    m_commands = std::make_shared<std::vector<std::string>>();

    auto job = [processor, old_commands, old_time]()
    {
        processor->process_data(old_commands, old_time);
    };
    progschj::ThreadPool::get_instance()->enqueue(job);
}

} // namespace my
