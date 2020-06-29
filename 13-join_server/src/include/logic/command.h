#ifndef COMMAND_H
#define COMMAND_H

#include "database.h"
#include "message.h"

#include <string>

namespace my
{

/**
 * @brief Run a command for the database
 * @param command[in] the command for the database
 * @return Response to be sent to user
 */
std::string run_command(const std::string& command);

} // namespace my

#endif // COMMAND_H
