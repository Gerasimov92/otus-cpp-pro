#pragma once

#include <ostream>
#include <list>
#include <tuple>
#include <type_traits>
#include <vector>

template<typename T, typename... Ts>
constexpr std::enable_if_t<std::conjunction_v<std::is_same<T, Ts>...>, bool> is_all_same = true;

/*!
 * Print IP-address, represented as an integer value.
 * \param out The output stream to print to.
 * \param ip The address to print.
 */
template<typename T,
         std::enable_if_t<std::is_integral_v<T>, bool> = true>
void print_ip(std::ostream &out, T ip)
{
    const auto *ptr = reinterpret_cast<std::uint8_t*>(&ip);
    for (auto i = sizeof(T); i > 0; i--)
    {
        out << static_cast<int>(ptr[i - 1]);
        if (i != 1)
            out << '.';
    }
}

/*!
 * Print IP-address, represented as a string value.
 * \param out The output stream to print to.
 * \param ip The address to print.
 */
template<typename T,
         std::enable_if_t<std::is_constructible_v<std::string, T>, bool> = true>
void print_ip(std::ostream &out, const T &ip)
{
    out << ip;
}

/*!
 * Print IP-address, represented as a vector or a list.
 * \param out The output stream to print to.
 * \param ip The address to print.
 */
template<typename T, typename VT = typename T::value_type, typename AT = typename T::allocator_type,
         std::enable_if_t<std::is_same_v<T, std::vector<VT, AT>> ||
                          std::is_same_v<T, std::list<VT, AT>>, bool> = true>
void print_ip(std::ostream &out, const T &ip)
{
    bool first = true;
    for (const auto &v : ip)
    {
        if (first)
            first = false;
        else
            out << '.';

        out << v;
    }
}

template<typename T, std::size_t... Is>
void print_tuple(std::ostream &os, const char *sep, const T &t, std::index_sequence<Is...>)
{
    ((os << (Is == 0 ? "" : sep) << std::get<Is>(t)), ...);
}

/*!
 * Print IP-address, represented as a tuple.
 * \warning All elements in a tuple must have the same type.
 * \param out The output stream to print to.
 * \param ip The address to print.
 */
template<typename... Args,
         std::enable_if_t<is_all_same<Args...>, bool> = true>
void print_ip(std::ostream &out, const std::tuple<Args...> &ip)
{
    print_tuple(out, ".", ip, std::make_index_sequence<sizeof...(Args)>());
}
