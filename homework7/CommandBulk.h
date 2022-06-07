#pragma once

#include "IVisitable.h"
#include "IVisitor.h"
#include <list>

class CommandBulk : public IVisitable
{
public:
    CommandBulk(const std::list<Command> &commands = {}) :
        m_commands(commands)
    {}

    void accept(IVisitor &visitor) override { visitor.visitCommandBulk(*this); }

    std::list<Command> commands() const { return m_commands; }

private:
    std::list<Command> m_commands;
};
