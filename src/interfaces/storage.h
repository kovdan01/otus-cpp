#ifndef STORAGE_H
#define STORAGE_H

#include "processor.h"

#include <string>

namespace my
{

class IStorage
{
public:
    virtual void add_processor(IProcessor* processor) = 0;
    virtual void remove_processor(IProcessor* processor) = 0;

    virtual void new_input(const std::string& input) = 0;
    virtual void end_input() = 0;

    virtual ~IStorage() = default;
};

} // namespace my

#endif // STORAGE_H
