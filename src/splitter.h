#ifndef SPLITTER_H
#define SPLITTER_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>

namespace my
{

class ISplitter
{
public:
    ISplitter(std::string filename)
        : m_filename(std::move(filename))
    {
    }

    using FileRange = std::pair<std::ifstream::pos_type, std::ifstream::pos_type>;
    virtual std::vector<FileRange> split(std::size_t count) const = 0;

protected:
    const std::string& filename() const
    {
        return m_filename;
    }

private:
    std::string m_filename;
};

class MySplitter : public ISplitter
{
public:
    using ISplitter::ISplitter;
    std::vector<FileRange> split(std::size_t count) const override;
};

} // namespace my

#endif // SPLITTER_H
