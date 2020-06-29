#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "interfaces/writer.h"

#include <string>

namespace my
{

/**
 * @brief The FileWriter class
 */
class FileWriter : public IWriter
{
public:
    FileWriter(std::string filename_prefix, std::string filename_suffix);

    /**
     * @brief Writes the given string to file named <prefix><current_time><suffix>
     * @param str[in] String to be written
     */
    void write(const std::string& str) override;

    /**
     * @brief Writes the given string to file named <prefix><time><suffix>
     * @param str[in] String to be written
     * @param time[in] Timestamp
     */
    void write(const std::string& str, std::uint64_t time);

    virtual ~FileWriter() = default;

private:
    std::string m_filename_prefix;
    std::string m_filename_suffix;
};

} // namespace my

#endif // FILE_WRITER_H
