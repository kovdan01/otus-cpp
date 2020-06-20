#include "controller.h"

#include <cassert>
#include <future>
#include <iterator>
#include <list>
#include <utility>

#include <iostream>

namespace my
{

Controller::Controller(splitter_t splitter, mapper_t mapper, std::size_t mapper_threads, reducer_t reducer, std::size_t reducer_threads)
    : m_splitter(std::move(splitter))
    , m_mapper(std::move(mapper))
    , m_mapper_threads(mapper_threads)
    , m_reducer(std::move(reducer))
    , m_reducer_threads(reducer_threads)
{
}

std::vector<std::string> Controller::map(const std::string& filename,
    const splitter_return_t& edges,
    progschj::ThreadPool& mapper_workers,
    std::size_t prefix_size)
{
    std::vector<mapper_result_t> mapped(m_mapper_threads);
    for (std::size_t i = 0; i < m_mapper_threads; ++i)
    {
        auto mapper_job = [this, i, &edges, &filename, prefix_size]() -> std::list<std::string>
        {
            return m_mapper(filename, edges[i].first, edges[i].second, prefix_size);
        };
        mapped[i] = mapper_workers.enqueue(mapper_job);
    }

    std::list<std::string> tmp;
    for (mapper_result_t& future_result : mapped)
    {
        tmp.merge(future_result.get());
    }

    std::vector<std::string> ans(std::move_iterator(tmp.begin()), std::move_iterator(tmp.end()));
    return ans;
}

bool Controller::reduce(const std::vector<std::string>& mapped_data, progschj::ThreadPool& reducer_workers)
{
    using It = std::vector<std::string>::const_iterator;
    auto edges = std::make_shared<std::vector<std::pair<It, It>>>(m_reducer_threads);

    std::ptrdiff_t step = mapped_data.size() / m_reducer_threads;
    It prev = mapped_data.begin(), next;
    for (std::size_t i = 0; i < m_reducer_threads; ++i)
    {
        next = (mapped_data.end() - prev < step ? mapped_data.end() : prev + step);
        while (next != mapped_data.end() && std::next(next) != mapped_data.end() && *std::next(next) == *next)
            ++next;
        (*edges)[i].first = prev;
        (*edges)[i].second = next;
        prev = next;
    }

    std::vector<std::future<bool>> reduced(m_reducer_threads);
    for (std::size_t i = 0; i < m_reducer_threads; ++i)
    {
        auto reducer_job = [this, i, edges]() -> bool
        {
            return m_reducer((*edges)[i].first, (*edges)[i].second);
        };
        reduced[i] = reducer_workers.enqueue(reducer_job);
    }
    for (std::future<bool>& future_result : reduced)
        if (future_result.get() == false)
            return false;
    return true;
}

std::size_t Controller::work(const std::string& filename)
{
    std::vector<std::pair<std::ifstream::pos_type, std::ifstream::pos_type>> edges = splitter(filename, m_mapper_threads);
    assert(edges.size() == m_mapper_threads);

    std::size_t prefix_length = 1;
    std::vector<std::string> mapped_data;

    try
    {
        progschj::ThreadPool mapper_workers(m_mapper_threads);
        progschj::ThreadPool reducer_workers(m_reducer_threads);

        for (;; ++prefix_length)
        {
            mapped_data = map(filename, edges, mapper_workers, prefix_length);
            if (reduce(mapped_data, reducer_workers))
                break;
        }
    }
    catch (const PrefixException&)
    {
        return std::size_t(-1);
    }
    return prefix_length;
}

} // namespace my
