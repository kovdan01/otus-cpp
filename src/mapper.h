#ifndef MAPPER_H
#define MAPPER_H

#include "splitter.h"

#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>

namespace my
{

template <typename ReturnType>
class IMapper
{
public:
    IMapper(std::string filename)
        : m_filename(std::move(filename))
    {
    }

    virtual std::list<ReturnType> operator()(ISplitter::FileRange range) const = 0;
    virtual ~IMapper() = default;

protected:
    const std::string& filename() const
    {
        return m_filename;
    }

private:
    std::string m_filename;
};

class MyMapper : public IMapper<std::string>
{
public:
    struct PrefixException : public std::exception
    {
    };

    MyMapper(std::string filename, std::size_t prefix_length);
    std::list<std::string> operator()(ISplitter::FileRange range) const override;

    virtual ~MyMapper() = default;

private:
    std::string read_from_file(ISplitter::FileRange range) const;
    std::ifstream m_file;
    std::size_t m_prefix_length;
};

//using mapper_t = std::function<std::list<std::string> (const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to, std::size_t prefix_size)>;
//std::list<std::string> mapper(const std::string& filename, std::ifstream::pos_type from, std::ifstream::pos_type to, std::size_t prefix_size);

} // namespace my

#endif // MAPPER_H
