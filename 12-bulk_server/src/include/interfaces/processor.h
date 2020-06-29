#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "writer.h"

#include <memory>
#include <vector>
#include <string>

namespace my
{

/**
 * @brief The IProcessor interface
 * Represents a logging strings processor
 */
class IProcessor
{
public:
    /**
     * @brief Add logger
     * @param writer[in] Pointer to logger to be added
     */
    virtual void add_writer(const IWriter* writer) = 0;

    /**
     * @brief Remove logger
     * @param writer[in] Pointer to logger to be removed
     */
    virtual void remove_writer(const IWriter* writer) = 0;

    /**
     * @brief Process strings vector
     * @param data[in] Shared pointer to vector with strings to be processed
     * @param first_command_time[in] Time of data appearance, needed for logger
     */
    virtual void process_data(std::shared_ptr<const std::vector<std::string>> data, std::uint64_t first_command_time) = 0;

    virtual ~IProcessor() = default;
};

} // namespace my

#endif // PROCESSOR_H
