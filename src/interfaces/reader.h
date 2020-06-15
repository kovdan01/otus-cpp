#ifndef READER_H
#define READER_H

#include "storage.h"

#include <memory>

namespace my
{

class IReader
{
public:
    virtual void add_storage(IStorage* storage) = 0;
    virtual void remove_storage(IStorage* storage) = 0;

    virtual void read() = 0;

    virtual ~IReader() = default;
};

} // namespace my

#endif // READER_H
