#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "interfaces/processor.h"
#include "bulk_command.h"

#include <unordered_set>

namespace my
{

class BulkCommandProcessor : public IProcessor
{
public:
    void add_writer(IWriter* writer) override;
    void remove_writer(IWriter* writer) override;

    void process_data(std::shared_ptr<const std::vector<std::string>> data) override;

    virtual ~BulkCommandProcessor() = default;

private:
    std::unordered_set<IWriter*> m_writers;
};

} // namespace my

#endif // COMMAND_PROCESSOR_H
