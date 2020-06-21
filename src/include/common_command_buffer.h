#ifndef COMMON_COMMAND_BUFFER_H
#define COMMON_COMMAND_BUFFER_H

#include "interfaces/processor.h"
#include "stream_reader.h"
#include "thread_pool.h"

#include <vector>
#include <memory>
#include <mutex>
#include <chrono>

#include <iostream>

namespace my
{

class CommonCommandBuffer
{
public:
    CommonCommandBuffer(const CommonCommandBuffer&) = delete;
    CommonCommandBuffer& operator=(const CommonCommandBuffer) = delete;
    CommonCommandBuffer(CommonCommandBuffer&&) = delete;
    CommonCommandBuffer& operator=(CommonCommandBuffer&&) = delete;

    static CommonCommandBuffer* get_instance(std::size_t bulk, IProcessor* processor)
    {
        static CommonCommandBuffer instance(bulk, processor);
        return &instance;
    }

    void add_command(const std::string& command)
    {
        m_commands->emplace_back(command);
        if (m_commands->size() == 1)
        {
            using namespace std::chrono;
            m_first_command_time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        }
        if (m_commands->size() == m_bulk)
        {
            to_processor();
        }
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

    std::size_t m_bulk;
    IProcessor* m_processor;
    std::uint64_t m_first_command_time = 0;

    std::shared_ptr<std::vector<std::string>> m_commands;

    std::mutex m_mutex;
};

} // namespace my

#endif // COMMON_COMMAND_BUFFER_H
