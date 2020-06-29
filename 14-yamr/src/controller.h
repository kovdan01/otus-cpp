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

/**
 * @brief The Controller class
 * Incapsulates map-reduce operations
 */
class Controller
{
public:
    Controller(std::size_t mapper_threads, std::size_t reducer_threads);

    /**
     * @brief work Does the job desctribed in task
     * @param filename[in] Filename to inspect
     * @return Minimum different prefix length for strings
     * int the file, std::size_t(-1) if there is no such length
     */
    std::size_t work(const std::string& filename);

private:
    std::vector<std::string>
    map(const std::vector<ISplitter::FileRange>& edges,
        const MyMapper& mapper,
        progschj::ThreadPool& mapper_workers);

    bool
    reduce(const std::vector<std::string>& mapped_data,
           const reducer_t& reducer,
           progschj::ThreadPool& reducer_workers);

    std::size_t m_mapper_threads;
    std::size_t m_reducer_threads;
};

} // namespace my

#endif // CONTROLLER_H
