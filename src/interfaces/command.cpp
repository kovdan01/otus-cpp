#include "command.h"

#include <utility>

namespace my
{

ICommand::ICommand(std::string name)
    : m_name(std::move(name))
{
}

std::string ICommand::name() const
{
    return m_name;
}

} // namespace my
