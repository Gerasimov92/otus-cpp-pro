#pragma once

#include "IVisitable.h"
#include "IVisitor.h"
#include <string>

class Command : public IVisitable
{
public:
    Command(std::uint64_t timestamp, const std::string &value) :
        m_timestamp(timestamp), m_value(value)
    {}

    void accept(IVisitor &visitor) override { visitor.visitCommand(*this); }

    std::uint64_t timestamp() const { return m_timestamp; }
    std::string value() const { return m_value; }

private:
    std::uint64_t m_timestamp;
    std::string m_value;
};
