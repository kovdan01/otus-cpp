#include "file_writer.h"

#include <chrono>
#include <fstream>
#include <filesystem>

namespace my
{

FileWriter::FileWriter(std::string filename_prefix, std::string filename_suffix)
    : m_filename_prefix(std::move(filename_prefix))
    , m_filename_suffix(std::move(filename_suffix))
{
}

void FileWriter::open_file(const std::string& base_filename, std::ofstream& file) const
{
    std::size_t i = 0;
    std::filesystem::path path(base_filename);
    std::lock_guard lock(m_filename_check_lock);
    while (std::filesystem::exists(path))
    {
        ++i;
        path = base_filename + std::to_string(i);
    }
    file.open(path);
}

void FileWriter::write(const std::string& str) const
{
    using namespace std::chrono;
    std::uint64_t time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    std::string filename = m_filename_prefix + std::to_string(time) + m_filename_suffix;
    std::ofstream file;
    open_file(filename, file);
    file << str;
}

void FileWriter::write(const std::string& str, std::uint64_t time) const
{
    std::string filename = m_filename_prefix + std::to_string(time) + m_filename_suffix;
    std::ofstream file;
    open_file(filename, file);
    file << str;
}

} // namespace my
