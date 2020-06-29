#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <functional>
#include <utility>

namespace my
{

template <typename T>
class FilteredVectorAdaptor
{
public:
    FilteredVectorAdaptor(const std::vector<T>& pool,
           std::function<bool(const T&)> predicate)
        : m_pool(pool)
        , m_predicate(std::move(predicate))
    {
    }

    class Iterator
    {
    private:
        friend class FilteredVectorAdaptor<T>;

        Iterator(typename std::vector<T>::const_iterator it, const FilteredVectorAdaptor* filter)
            : m_it(it)
            , m_filter(filter)
        {
            while (m_it != m_filter->m_pool.cend() && !m_filter->m_predicate(*m_it))
            {
                ++m_it;
            }
        }

    public:
        Iterator& operator++()
        {
            ++m_it;
            while (m_it != m_filter->m_pool.cend() && !m_filter->m_predicate(*m_it))
            {
                ++m_it;
            }
            return *this;
        }

        const T& operator*() const
        {
            return *m_it;
        }

        bool operator!=(const Iterator& other)
        {
            return m_it != other.m_it;
        }

    private:
        typename std::vector<T>::const_iterator m_it;
        const FilteredVectorAdaptor* m_filter;
    };

    [[nodiscard]] Iterator begin() const
    {
        return Iterator{ m_pool.cbegin(), this };
    }

    [[nodiscard]] Iterator end() const
    {
        return Iterator{ m_pool.cend(), this };
    }

private:
    const std::vector<T>& m_pool;
    std::function<bool(const T&)> m_predicate;
};

} // namespace my

#endif // FILTER_H
