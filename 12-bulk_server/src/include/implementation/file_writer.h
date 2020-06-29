#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "interfaces/writer.h"

#include <string>
#include <mutex>

namespace my
{

/**
 * @brief The FileWriter class
 */
class FileWriter : public IWriter
{
public:
    FileWriter(std::string filename_prefix, std::string filename_suffix,
               std::mutex* filename_check_lock);

    /**
     * @brief Writes the given string to file named <prefix><current_time><suffix>
     * @param str[in] String to be written
     */
    void write(const std::string& str) const override;

    /**
     * @brief Writes the given string to file named <prefix><time><suffix>
     * @param str[in] String to be written
     * @param time[in] Timestamp
     */
    void write(const std::string& str, std::uint64_t time) const;

    virtual ~FileWriter() = default;

private:
    void open_file(const std::string& base_filename, std::ofstream& file) const;
    void open_file_non_blocking(const std::string& base_filename, std::ofstream& file) const;

    std::string m_filename_prefix;
    std::string m_filename_suffix;
    mutable std::mutex* m_filename_check_lock;
};

} // namespace my

#endif // FILE_WRITER_H
