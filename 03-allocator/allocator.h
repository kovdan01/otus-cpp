#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <array>
#include <stdexcept>
#include <cassert>

namespace my
{

template <typename T, std::size_t size>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    Allocator()
    {
        m_data = reinterpret_cast<T*>(std::malloc(size * sizeof(T)));
        if (m_data == nullptr)
            throw std::bad_alloc();
        m_memory_map.fill(false);
    }

    ~Allocator()
    {
        std::free(m_data);
    }

    template <typename U>
    struct rebind
    {
        using other = Allocator<U, size>;
    };

    T* allocate(std::size_t n)
    {
        if (size - m_allocated_size < n)
            throw std::bad_alloc();

        std::size_t from = 0, to = 0;
        for (std::size_t i = 0; i < size; ++i)
        {
            if (m_memory_map[i])
                continue;
            std::size_t j = i + 1;
            for (; j < size && !m_memory_map[j] && j - i + 1 <= n; ++j) {}
            if (j - i == n)
            {
                from = i;
                to = j;
                break;
            }
        }
        if (from == 0 && to == 0)
            throw std::bad_alloc();

        for (std::size_t i = from; i < to; ++i)
            m_memory_map[i] = true;
        m_allocated_size += n;
        return m_data + from;
    }

    void deallocate(T* p, std::size_t n)
    {
        assert(n <= m_allocated_size);
        std::size_t from = p - m_data;
        assert(from + n <= size);

        for (std::size_t i = 0; i < n; ++i, ++p)
            m_memory_map[from + i] = false;

        m_allocated_size -= n;
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    void destroy(T* p)
    {
        p->~T();
    }

private:
    T* m_data = nullptr;
    std::size_t m_allocated_size = 0;
    std::array<bool, size> m_memory_map;
};

} // namespace my

#endif // ALLOCATOR_H
