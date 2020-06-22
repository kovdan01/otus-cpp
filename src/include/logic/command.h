#ifndef COMMAND_H
#define COMMAND_H

#include "database.h"
#include "message.h"

#include <string>

namespace my
{

std::string run_command(const std::string& command);

} // namespace my

#endif // COMMAND_H
