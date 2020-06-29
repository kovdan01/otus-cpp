#include "file_writer.h"

#include <chrono>
#include <fstream>

namespace my
{

FileWriter::FileWriter(std::string filename_prefix, std::string filename_suffix)
    : m_filename_prefix(std::move(filename_prefix))
    , m_filename_suffix(std::move(filename_suffix))
{
}

void FileWriter::write(const std::string& str)
{
    using namespace std::chrono;
    std::uint64_t time = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
    std::string filename = m_filename_prefix + std::to_string(time) + m_filename_suffix;
    std::ofstream file(filename);
    file << str;
}

void FileWriter::write(const std::string& str, std::uint64_t time)
{
    std::string filename = m_filename_prefix + std::to_string(time) + m_filename_suffix;
    std::ofstream file(filename);
    file << str;
}

} // namespace my
