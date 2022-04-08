#include "IPv4Address.h"
#include "IPv4Filter.h"

#include <algorithm>
#include <string>
#include <vector>

void IPv4Filter::filter(std::istream &in, std::ostream &out)
{
    std::vector<IPv4Address> addresses;
    for (std::string line; std::getline(in, line); )
    {
        addresses.emplace_back(line.substr(0, line.find('\t')));
    }

    auto print = [&out](const auto &value) { out << value << std::endl; };

    auto print_if = [print](const auto &container, auto &&func)
    {
        std::for_each(container.cbegin(), container.cend(),
                      [print, func](const auto &item) { if (func(item)) print(item); });
    };

    auto contains = [](const IPv4Address &addr, std::uint8_t byte)
    {
        const auto &bytes = addr.bytes();
        return std::find(bytes.cbegin(), bytes.cend(), byte) != bytes.cend();
    };

    std::sort(addresses.begin(), addresses.end(), std::greater<>());
    std::for_each(addresses.cbegin(), addresses.cend(), print);
    print_if(addresses, [](const IPv4Address &addr) { return addr[0] == 1; });
    print_if(addresses, [](const IPv4Address &addr) { return addr[0] == 46 && addr[1] == 70; });
    print_if(addresses, std::bind(contains, std::placeholders::_1, 46));
}
