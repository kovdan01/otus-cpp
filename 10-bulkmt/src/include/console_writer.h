#ifndef CONSOLE_WRITER_H
#define CONSOLE_WRITER_H

#include "interfaces/writer.h"

namespace my
{

/**
 * @brief The ConsoleWriter class
 * Represents a dummy stdout writer
 */
class ConsoleWriter : public IWriter
{
public:
    void write(const std::string& str) const override;

    virtual ~ConsoleWriter() = default;
};

} // namespace my

#endif // CONSOLE_WRITER_H
