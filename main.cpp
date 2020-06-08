#include "allocator.h"
#include "list.h"

#include <map>
#include <functional>
#include <iostream>

template <typename Integer>
Integer factorial(Integer number)
{
    Integer ans = 1;
    for (Integer i = 2; i <= number; ++i)
        ans *= i;
    return ans;
}

template <typename Allocator>
void fill_map(std::map<int, int, std::less<int>, Allocator>& map)
{
    for (int i = 0; i < 10; ++i)
        map.emplace(i, factorial(i));
}

template <typename Allocator>
void fill_list(my::ForwardList<int, Allocator>& list)
{
    auto last = list.begin();
    for (int i = 0; i < 10; ++i)
        last = list.insert_after(last, i);
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair)
{
    stream << pair.first << ' ' << pair.second;
    return stream;
}

template <typename Container>
void print_container(const Container& container)
{
    for (const auto& element : container)
        std::cout << element << '\n';
}

int main()
{
    std::map<int, int> map_std_alloc;
    std::map<int, int, std::less<int>, my::Allocator<std::pair<const int, int>, 10>> map_my_alloc;
    my::ForwardList<int> list_std_alloc;
    my::ForwardList<int, my::Allocator<int, 10>> list_my_alloc;

    fill_map(map_std_alloc);
    fill_map(map_my_alloc);
    fill_list(list_std_alloc);
    fill_list(list_my_alloc);

    std::cout << "\nmap_std_alloc:\n";
    print_container(map_std_alloc);
    std::cout << "\nmap_my_alloc:\n";
    print_container(map_my_alloc);
    std::cout << "\nlist_std_alloc:\n";
    print_container(list_std_alloc);
    std::cout << "\nlist_my_alloc:\n";
    print_container(list_my_alloc);
}
