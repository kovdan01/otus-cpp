#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "writer.h"

#include <memory>
#include <vector>
#include <string>

namespace my
{

class IProcessor
{
public:
    virtual void add_writer(IWriter* writer) = 0;
    virtual void remove_writer(IWriter* writer) = 0;

    virtual void process_data(std::shared_ptr<const std::vector<std::string>> data) = 0;

    virtual ~IProcessor() = default;
};

} // namespace my

#endif // PROCESSOR_H
