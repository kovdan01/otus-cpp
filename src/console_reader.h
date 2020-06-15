#ifndef CONSOLE_READER_H
#define CONSOLE_READER_H

#include "interfaces/reader.h"

#include <unordered_set>
#include <string>

namespace my
{

class ConsoleReader : public IReader
{
public:
    ConsoleReader(std::size_t block_size);

    void add_storage(IStorage* storage) override;
    void remove_storage(IStorage* storage) override;

    void read() override;

    virtual ~ConsoleReader();

private:
    void open_brace();
    void close_brace();

    void notify_end_input();
    void notify_new_line(const std::string& line);

    std::unordered_set<IStorage*> m_storages;
    std::size_t m_block_size;
    std::size_t m_lines_accumulated = 0;
    unsigned int m_braces_level = 0;
};

} // namespace my

#endif // CONSOLE_READER_H
