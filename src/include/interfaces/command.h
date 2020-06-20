#ifndef COMMAND_H
#define COMMAND_H

#include <string>

namespace my
{

/**
 * @brief The ICommand interface
 * Represents a simple command
 */
class ICommand
{
public:
    /**
     * @brief Constructor from name
     * @param name Command name
     */
    ICommand(std::string name);

    /**
     * @brief Command execute virtual method
     * Override it with your custom behavior
     */
    virtual void execute() = 0;

    /**
     * @brief Command name getter
     * @return Command name
     */
    virtual std::string name() const final;

    virtual ~ICommand() = default;

private:
    std::string m_name;
};

} // namespace my

#endif // COMMAND_H
