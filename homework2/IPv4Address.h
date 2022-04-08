#pragma once

#include <array>
#include <ostream>
#include <regex>
#include <string>

class IPv4Address
{
public:
    explicit IPv4Address(const std::string &address = {});

    const std::array<std::uint8_t, 4> &bytes() const { return m_bytes; }
    const std::uint8_t &operator[](std::size_t index) const;
    inline bool operator>(const IPv4Address &other) const { return m_bytes > other.m_bytes; }

    std::string toString() const;

private:
    std::array<std::uint8_t, 4> m_bytes;
};

std::ostream &operator<<(std::ostream &out, const IPv4Address &address);
