#include "implementation/file_writer.h"
#include "implementation/global_state.h"
#include "utils/utils.h"

#include <fstream>
#include <filesystem>

namespace my
{

FileWriter::FileWriter(std::string filename_prefix, std::string filename_suffix, std::mutex* filename_check_lock)
    : m_filename_prefix(std::move(filename_prefix))
    , m_filename_suffix(std::move(filename_suffix))
    , m_filename_check_lock(filename_check_lock)
{
}

void FileWriter::open_file_non_blocking(const std::string& base_filename, std::ofstream& file) const
{
    std::size_t i = 0;
    std::filesystem::path path(base_filename);
    while (std::filesystem::exists(path))
    {
        ++i;
        path = base_filename + std::to_string(i);
    }
    file.open(path);
}

void FileWriter::open_file(const std::string& base_filename, std::ofstream& file) const
{
    if (m_filename_check_lock != nullptr)
    {
        std::lock_guard lock(*m_filename_check_lock);
        open_file_non_blocking(base_filename, file);
    }
    else
    {
        open_file_non_blocking(base_filename, file);
    }
}

void FileWriter::write(const std::string& str) const
{
    std::uint64_t time = utils::get_current_time();
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
