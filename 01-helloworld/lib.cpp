#include "lib.h"
#include "version.h"

Version version()
{
    return {PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH};
}

std::string message()
{
    return "Hello world!\n";
}
