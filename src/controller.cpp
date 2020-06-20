#include "controller.h"

#include <utility>
#include <cassert>
#include <future>
#include <list>
#include <iterator>


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

std::pair<std::vector<std::string>, bool> Controller::map(const std::string& filename,
                                         const splitter_return_t& edges,
                                         progschj::ThreadPool& mapper_workers,
                                         std::size_t prefix_size)
{
    std::vector<mapper_result_t> mapped(m_mapper_threads);
    for (std::size_t i = 0; i < m_mapper_threads; ++i)
    {
        auto mapper_job = [this, i, &edges, &filename, prefix_size]() -> std::pair<std::list<std::string>, bool>
        {
            return m_mapper(filename, edges[i].first, edges[i].second, prefix_size);
        };
        mapped[i] = mapper_workers.enqueue(mapper_job);
    }
    for (mapper_result_t& future_result : mapped)
        future_result.wait();

    std::list<std::string> tmp;
    for (mapper_result_t& future_result : mapped)
    {
        auto e = future_result.get();
        if (!e.second)
            return {{}, false};
        tmp.merge(std::move(e.first));
    }

    std::vector<std::string> ans(std::move_iterator(tmp.begin()), std::move_iterator(tmp.end()));
    return {std::move(ans), true};
}

bool Controller::reduce(const std::vector<std::string>& mapped_data, progschj::ThreadPool& reducer_workers)
{
    using It = std::vector<std::string>::const_iterator;
    std::vector<std::pair<It, It>> edges(m_reducer_threads);

    //std::cout << "AAA" << std::endl;
    std::ptrdiff_t step = mapped_data.size() / m_reducer_threads;
    It prev = mapped_data.begin(), next;
    for (std::size_t i = 0; i < m_reducer_threads; ++i)
    {
        //std::cout << "B. i = " << i << std::endl;
        next = (mapped_data.end() - prev < step ? mapped_data.end() : prev + step);
        while (std::next(next) != mapped_data.end() && *std::next(next) == *next)
            ++next;
        edges[i].first = prev;
        edges[i].second = next;
        prev = next;
    }

    std::vector<std::future<bool>> reduced(m_reducer_threads);
    for (std::size_t i = 0; i < m_reducer_threads; ++i)
    {
        //std::cout << "C. i = " << i << std::endl;
        auto reducer_job = [this, i, &edges]() -> bool
        {
            return m_reducer(edges[i].first, edges[i].second);
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
    //std::vector<mapper_result_t> mapped(m_mapper_threads);
    std::vector<std::string> mapped_data;

    progschj::ThreadPool mapper_workers(m_mapper_threads);
    progschj::ThreadPool reducer_workers(m_reducer_threads);

    for (;; ++prefix_length)
    {
        auto [mapped_data, ok] = map(filename, edges, mapper_workers, prefix_length);
        if (!ok)
            return std::size_t(-1);
        std::cout << "Mapped data" << std::endl;
        for (const auto& elem : mapped_data)
            std::cout << elem << std::endl;
        bool ans = reduce(mapped_data, reducer_workers);
        if (ans)
            break;
    }
    return prefix_length;
}

} // namespace my
