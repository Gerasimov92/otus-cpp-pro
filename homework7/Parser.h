#pragma once

#include <istream>
#include <list>
#include <memory>

class IVisitable;

class Parser
{
public:
    Parser(std::istream &input, size_t maxBulkSize);

    std::unique_ptr<IVisitable> parse();

private:
    std::uint64_t currentTime();
    std::string parseToken(std::istream &input);

    std::istream &m_input;
    size_t m_maxBulkSize;
    int m_nestingLevel;
};

