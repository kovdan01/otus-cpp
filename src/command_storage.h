#ifndef COMMAND_STORAGE_H
#define COMMAND_STORAGE_H

#include "interfaces/storage.h"
#include "interfaces/processor.h"

#include <vector>
#include <memory>
#include <string>
#include <unordered_set>

namespace my
{

class CommandStorage : public IStorage
{
public:
    CommandStorage();

    void add_processor(IProcessor* processor) override;
    void remove_processor(IProcessor* processor) override;

    void new_input(const std::string& input) override;
    void end_input() override;

    virtual ~CommandStorage() = default;

private:
    std::shared_ptr<std::vector<std::string>> m_commands;
    std::unordered_set<IProcessor*> m_processors;
    std::uint64_t m_first_command_time = 0;
};

} // namespace my

#endif // COMMAND_STORAGE_H
