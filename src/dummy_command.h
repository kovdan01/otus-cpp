#ifndef BULK_COMMAND_H
#define BULK_COMMAND_H

#include "interfaces/command.h"

namespace my
{

/**
 * @brief The DummyCommand class
 * Represents a dummy command, that does nothing
 */
class DummyCommand : public ICommand
{
public:
    using ICommand::ICommand;

    void execute() override;

    virtual ~DummyCommand() = default;
};

} // namespace my

#endif // BULK_COMMAND_H
