#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "interfaces/processor.h"
#include "interfaces/writer.h"
#include "file_writer.h"

#include <unordered_set>

namespace my
{

class BulkCommandProcessor : public IProcessor
{
public:
    void add_writer(IWriter* writer) override;
    void add_writer(FileWriter* writer);
    void remove_writer(IWriter* writer) override;

    void process_data(std::shared_ptr<const std::vector<std::string>> data, std::uint64_t first_command_time) override;

    virtual ~BulkCommandProcessor() = default;

private:
    std::unordered_set<IWriter*> m_writers;
    std::unordered_set<FileWriter*> m_file_writers;
};

} // namespace my

#endif // COMMAND_PROCESSOR_H
