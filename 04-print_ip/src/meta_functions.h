#ifndef META_FUNCTIONS_H
#define META_FUNCTIONS_H

#include <type_traits>
#include <string>

namespace my
{

namespace meta
{

template<typename T>
struct has_iterator
{
private:
    using yes =   signed char;
    using  no = unsigned char;

    template <typename U>
    static yes test(typename U::iterator*);

    template <typename U>
    static no  test(...);

public:
    static constexpr bool value = std::is_same_v<decltype(test<T>(nullptr)), yes>;
};

template <typename T>
inline constexpr bool has_iterator_v = has_iterator<T>::value;

template <typename T>
struct is_string
{
    static constexpr bool value = false;
};

template <>
struct is_string<std::string>
{
    static constexpr bool value = true;
};

template <typename T>
inline constexpr bool is_string_v = is_string<T>::value;

template <typename T, typename... Args>
struct split_pack
{
    using first = T;
};

template <typename... Args>
struct check_tuple
{
    static constexpr bool value = true;
};

template <typename T>
struct check_tuple<T>
{
    static constexpr bool value = true;
};

template <typename T, typename U, typename... Args>
struct check_tuple<T, U, Args...>
{
    static constexpr bool value = !std::is_same_v<T, U>
                                      ? false
                                      : check_tuple<U, Args...>::value;
};

template <typename... Args>
inline constexpr bool check_tuple_v = check_tuple<Args...>::value;

} // namespace detail

} // namespace my

#endif // META_FUNCTIONS_H
