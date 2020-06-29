#ifndef SPLITTER_H
#define SPLITTER_H

#include <fstream>
#include <string>
#include <vector>

namespace my
{

/**
 * @brief The ISplitter interface
 * Interface to split a file
 */
class ISplitter
{
public:
    ISplitter(std::string filename)
        : m_filename(std::move(filename))
    {
    }

    using FileRange = std::pair<std::ifstream::pos_type, std::ifstream::pos_type>;
    [[nodiscard]] virtual std::vector<FileRange> split(std::size_t count) const = 0;

    virtual ~ISplitter() = default;

protected:
    [[nodiscard]] const std::string& filename() const
    {
        return m_filename;
    }

private:
    std::string m_filename;
};

/**
 * @brief The MySplitter class
 * Splits files by newline character
 */
class MySplitter : public ISplitter
{
public:
    using ISplitter::ISplitter;

    /**
     * @brief split Splits the file to parts of approximately equal
     * length. The parts are forced to start and end with a new line.
     * @param count Number of parts to split into
     * @return File position ranges
     */
    [[nodiscard]] std::vector<FileRange> split(std::size_t count) const override;

    virtual ~MySplitter() = default;
};

} // namespace my

#endif // SPLITTER_H
