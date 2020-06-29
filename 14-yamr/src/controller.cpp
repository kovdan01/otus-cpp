#include "controller.h"

#include <cassert>
#include <future>
#include <iterator>
#include <vector>
#include <list>

namespace my
{

Controller::Controller(std::size_t mapper_threads, std::size_t reducer_threads)
    : m_mapper_threads(mapper_threads)
    , m_reducer_threads(reducer_threads)
{
}

std::vector<std::string>
Controller::map(const std::vector<ISplitter::FileRange>& edges,
                const MyMapper& mapper,
                progschj::ThreadPool& mapper_workers)
{
    using mapper_result_t = std::future<std::list<std::string>>;
    std::vector<mapper_result_t> mapped(m_mapper_threads);
    for (std::size_t i = 0; i < m_mapper_threads; ++i)
    {
        auto mapper_job = [i, &edges, &mapper]() -> std::list<std::string>
        {
            return mapper(edges[i]);
        };
        mapped[i] = mapper_workers.enqueue(mapper_job);
    }

    std::list<std::string> tmp;
    bool was_exception = false;
    for (mapper_result_t& future_result : mapped)
    {
        try
        {
            tmp.merge(future_result.get());
        }
        // need to get all the std::future instances, otherwise mapper
        // will be destroyed and an access to destroyed object will occur
        catch (const MyMapper::PrefixException&)
        {
            was_exception = true;
        }
    }
    if (was_exception)
        throw MyMapper::PrefixException{};

    std::vector<std::string> ans(std::move_iterator(tmp.begin()), std::move_iterator(tmp.end()));
    return ans;
}

bool
Controller::reduce(const std::vector<std::string>& mapped_data,
                   const reducer_t& reducer,
                   progschj::ThreadPool& reducer_workers)
{
    using It = std::vector<std::string>::const_iterator;
    auto edges = std::vector<std::pair<It, It>>(m_reducer_threads);

    std::ptrdiff_t step = mapped_data.size() / m_reducer_threads;
    It prev = mapped_data.begin(), next;
    for (std::size_t i = 0; i < m_reducer_threads; ++i)
    {
        next = (mapped_data.end() - prev < step ? mapped_data.end() : prev + step);
        if (next == mapped_data.end())
        {
            edges[i].first = prev;
            edges[i].second = next;
            prev = next;
            continue;
        }
        while (std::next(next) != mapped_data.end() && *std::next(next) == *next)
            ++next;
        edges[i].first = prev;
        edges[i].second = std::next(next);
        prev = std::next(next);
    }

    std::vector<std::future<bool>> reduced(m_reducer_threads);
    for (std::size_t i = 0; i < m_reducer_threads; ++i)
    {
        auto reducer_job = [reducer, i, &edges]() -> bool
        {
            return reducer(edges[i].first, edges[i].second);
        };
        reduced[i] = reducer_workers.enqueue(reducer_job);
    }

    bool answer = true;
    for (std::future<bool>& future_result : reduced)
    {
        // cannot return immidiately, need to get() all the futures
        // because they use references to edges and mapped_data
        if (!future_result.get())
            answer = false;
    }
    return answer;
}

std::size_t Controller::work(const std::string& filename)
{
    std::vector<ISplitter::FileRange> edges = MySplitter(filename).split(m_mapper_threads);
    assert(edges.size() == m_mapper_threads);

    std::size_t prefix_length = 1;
    std::vector<std::string> mapped_data;

    try
    {
        progschj::ThreadPool mapper_workers(m_mapper_threads);
        progschj::ThreadPool reducer_workers(m_reducer_threads);

        for (;; ++prefix_length)
        {
            {
                MyMapper mapper(filename, prefix_length);
                mapped_data = map(edges, mapper, mapper_workers);
            }
            if (reduce(mapped_data, &reducer, reducer_workers))
                break;
        }
    }
    catch (const MyMapper::PrefixException&)
    {
        return std::size_t(-1);
    }
    return prefix_length;
}

} // namespace my
