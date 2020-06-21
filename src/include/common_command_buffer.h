#ifndef COMMON_COMMAND_BUFFER_H
#define COMMON_COMMAND_BUFFER_H

#include "interfaces/processor.h"
#include "stream_reader.h"
#include "thread_pool.h"
#include "utils.h"

#include <vector>
#include <memory>
#include <mutex>

namespace my
{

class CommonCommandBuffer
{
public:
    CommonCommandBuffer(std::size_t bulk, IProcessor* processor)
        : m_bulk(bulk)
        , m_processor(processor)
        , m_commands(std::make_shared<std::vector<std::string>>())
    {
    }

    ~CommonCommandBuffer()
    {
        if (!m_commands->empty())
            m_processor->process_data(m_commands, m_first_command_time);
    }

    void add_command(const std::string& command)
    {
        m_commands->emplace_back(command);
        if (m_commands->size() == 1)
            m_first_command_time = utils::get_current_time();
        if (m_commands->size() == m_bulk)
            to_processor();
    }

    void to_processor()
    {
        std::shared_ptr<std::vector<std::string>> old_commands = m_commands;
        std::uint64_t old_time = m_first_command_time;
        m_commands = std::make_shared<std::vector<std::string>>();
        if (!old_commands->empty())
        {
            IProcessor* processor = m_processor;
            auto job = [processor, old_commands, old_time]()
            {
                processor->process_data(old_commands, old_time);
            };
            progschj::ThreadPool::get_instance()->enqueue(job);
        }
    }

private:
    std::size_t m_bulk;
    IProcessor* m_processor;
    std::uint64_t m_first_command_time = 0;
    std::shared_ptr<std::vector<std::string>> m_commands;
};

} // namespace my

#endif // COMMON_COMMAND_BUFFER_H
