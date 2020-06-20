#ifndef STORAGE_H
#define STORAGE_H

#include "processor.h"

#include <string>

namespace my
{

/**
 * @brief The IStorage interface
 * Represents a storage for strings
 */
class IStorage
{
public:
    /**
     * @brief Add processor to process storage data
     * @param processor[in] Pointer to processor to be added
     */
    virtual void add_processor(IProcessor* processor) = 0;

    /**
     * @brief Remove processor
     * @param processor[in] Pointer to processor to be removed
     */
    virtual void remove_processor(IProcessor* processor) = 0;

    /**
     * @brief The function that an observable reader calls when new input appears
     * @param input[in] New string from reader
     */
    virtual void new_input(const std::string& input) = 0;

    /**
     * @brief The function that an observable reader calls on the input end
     */
    virtual void end_input() = 0;

    virtual ~IStorage() = default;
};

} // namespace my

#endif // STORAGE_H
