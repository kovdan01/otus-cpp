#ifndef WRITER_H
#define WRITER_H

#include <string>

namespace my
{

/**
 * @brief The IWriter interface
 */
class IWriter
{
public:
    /**
     * @brief Write string
     * @param str[in] String to be written
     */
    virtual void write(const std::string& str) const = 0;

    virtual ~IWriter() = default;
};

} // namespace my

#endif // WRITER_H
