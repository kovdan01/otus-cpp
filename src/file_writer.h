#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "interfaces/writer.h"

#include <string>

namespace my
{

class FileWriter : public IWriter
{
public:
    FileWriter(std::string filename_prefix, std::string filename_suffix);

    void write(const std::string& str) override;
    void write(const std::string& str, std::uint64_t time);

    virtual ~FileWriter() = default;

private:
    std::string m_filename_prefix;
    std::string m_filename_suffix;
};

} // namespace my

#endif // FILE_WRITER_H
