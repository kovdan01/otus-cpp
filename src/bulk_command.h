#ifndef BULK_COMMAND_H
#define BULK_COMMAND_H

#include "interfaces/command.h"

namespace my
{

class BulkCommand : public ICommand
{
public:
    using ICommand::ICommand;

    void execute() override;

    virtual ~BulkCommand() = default;
};

} // namespace my

#endif // BULK_COMMAND_H
