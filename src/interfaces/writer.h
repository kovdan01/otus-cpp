#ifndef WRITER_H
#define WRITER_H

#include <string>

namespace my
{

class IWriter
{
public:
    virtual void write(const std::string& str) = 0;

    virtual ~IWriter() = default;
};

} // namespace my

#endif // WRITER_H
