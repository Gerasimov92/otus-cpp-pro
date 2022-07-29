#include "Parser.h"
#include "Command.h"
#include "CommandBulk.h"
#include <chrono>

Parser::Parser(std::istream &input, size_t maxBulkSize) :
    m_input(input),
    m_maxBulkSize(maxBulkSize),
    m_nestingLevel(0)
{}

std::unique_ptr<IVisitable> Parser::parse(bool combineStaticBlocks)
{
    std::list<Command> commands;
    std::string token;

    while (1)
    {
        token = parseToken(m_input);
        if (token.empty())
        {
            if (m_nestingLevel > 0 || commands.empty())
                return nullptr;
            else
                break;
        }

        if (token == "{")
        {
            m_nestingLevel++;
            if (m_nestingLevel == 1 && !commands.empty())
                break;
            else
                continue;
        }
        else if (token == "}")
        {
            m_nestingLevel--;
            if (m_nestingLevel == 0)
                break;
            else
                continue;
        }

        if (!combineStaticBlocks && commands.empty())
            return std::make_unique<Command>(currentTime(), token);

        commands.push_back(Command(currentTime(), token));

        if (m_nestingLevel == 0 && commands.size() == m_maxBulkSize)
            break;
    }

    return std::make_unique<CommandBulk>(commands);
}

uint64_t Parser::currentTime()
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}

std::string Parser::parseToken(std::istream &input)
{
    std::string line;
    input >> line;
    return line;
}
