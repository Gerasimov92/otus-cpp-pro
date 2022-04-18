#include "IPv4Address.h"
#include <algorithm>
#include <cassert>
#include <sstream>

IPv4Address::IPv4Address(const std::string &address) :
    m_bytes{0, 0, 0, 0}
{
    std::regex re("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                  "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                  "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                  "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");

    std::smatch match;
    if (!std::regex_match(address, match, re))
        return;

    std::transform(match.cbegin() + 1, match.cend(), m_bytes.begin(),
                   [](const std::string &token) { return std::stoul(token); });
}

const uint8_t &IPv4Address::operator[](std::size_t index) const
{
    assert(index < m_bytes.size());
    return m_bytes[index];
}

std::string IPv4Address::toString() const
{
    std::ostringstream out;

    for (int i = 0; i < m_bytes.size(); i++)
    {
        out << static_cast<std::uint32_t>(m_bytes[i]);
        if (i < m_bytes.size() - 1)
            out << ".";
    }

    return out.str();
}

std::ostream &operator<<(std::ostream &out, const IPv4Address &address)
{
    return out << address.toString();
}
