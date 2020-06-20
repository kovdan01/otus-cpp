#ifndef MAPPER_H
#define MAPPER_H

#include "splitter.h"

#include <string>
#include <list>
#include <stdexcept>

namespace my
{

/**
 * @brief The IMapper interface
 * Interface to preprocess data (map step) from a given file
 */
template <typename ReturnType>
class IMapper
{
public:
    IMapper(std::string filename)
        : m_filename(std::move(filename))
    {
    }

    /**
     * @brief operator () Perform map (data preprocessing) operation
     * @param range Range to file positions to take data from
     * @return Preprocessed data
     */
    virtual std::list<ReturnType> operator()(ISplitter::FileRange range) const = 0;
    virtual ~IMapper() = default;

protected:
    [[nodiscard]] const std::string& filename() const
    {
        return m_filename;
    }

private:
    std::string m_filename;
};

/**
 * @brief The MyMapper class
 * Takes a prefix from each string
 */
class MyMapper : public IMapper<std::string>
{
public:
    /**
     * @brief The PrefixException struct
     * Throwed when prefix_length is greater then length of one
     * of the strings while performing map operation
     */
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

} // namespace my

#endif // MAPPER_H
