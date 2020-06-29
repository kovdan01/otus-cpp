#ifndef READER_H
#define READER_H

#include "storage.h"

#include <memory>

namespace my
{

/**
 * @brief The IReader interface
 */
class IReader
{
public:
    /**
     * @brief Add storage to put strings that are read
     * @param storage[in] Pointer to storage to be added
     */
    virtual void add_storage(IStorage* storage) = 0;

    /**
     * @brief Remove storage
     * @param storage[in] Pointer to storage to be removed
     */
    virtual void remove_storage(IStorage* storage) = 0;

    /**
     * @brief Read strings
     */
    virtual void read() = 0;

    virtual ~IReader() = default;
};

} // namespace my

#endif // READER_H
