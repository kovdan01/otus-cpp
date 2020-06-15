#ifndef CONSOLE_WRITER_H
#define CONSOLE_WRITER_H

#include "interfaces/writer.h"

namespace my
{

class ConsoleWriter : public IWriter
{
public:
    void write(const std::string& str) override;

    virtual ~ConsoleWriter() = default;
};

} // namespace my

#endif // CONSOLE_WRITER_H
