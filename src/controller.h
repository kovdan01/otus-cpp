#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mapper.h"
#include "reducer.h"
#include "splitter.h"
#include "thread_pool.h"

#include <string>
#include <vector>

namespace my
{

class Controller
{
public:
    Controller(splitter_t splitter,
        mapper_t mapper_wrapper, std::size_t mapper_threads,
        reducer_t reducer, std::size_t reducer_threads);

    std::size_t work(const std::string& filename);

private:
    using mapper_result_t = std::future<std::list<std::string>>;
    std::vector<std::string> map(const std::string& filename,
        const splitter_return_t& edges,
        progschj::ThreadPool& mapper_workers,
        std::size_t prefix_size);

    bool reduce(const std::vector<std::string>& mapped_data, progschj::ThreadPool& reducer_workers);

    splitter_t m_splitter;

    mapper_t m_mapper;
    std::size_t m_mapper_threads;

    reducer_t m_reducer;
    std::size_t m_reducer_threads;
};

} // namespace my

#endif // CONTROLLER_H
