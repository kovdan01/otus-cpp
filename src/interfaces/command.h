#ifndef COMMAND_H
#define COMMAND_H

#include <string>

namespace my
{

class ICommand
{
public:
    ICommand(std::string name);

    virtual void execute() = 0;

    virtual std::string name() const final;

    virtual ~ICommand() = default;

private:
    std::string m_name;
};

} // namespace my

#endif // COMMAND_H
