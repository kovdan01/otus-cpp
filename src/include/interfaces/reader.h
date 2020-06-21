#ifndef READER_H
#define READER_H

#include "command_storage.h"

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
     * @brief Read strings
     */
    virtual void read() = 0;

    virtual ~IReader() = default;
};

} // namespace my

#endif // READER_H
