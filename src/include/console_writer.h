#ifndef CONSOLE_WRITER_H
#define CONSOLE_WRITER_H

#include "interfaces/writer.h"

#include <mutex>

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

private:
    mutable std::mutex m_mutex;
};

} // namespace my

#endif // CONSOLE_WRITER_H
