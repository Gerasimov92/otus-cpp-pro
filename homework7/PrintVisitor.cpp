#include "PrintVisitor.h"
#include "Command.h"
#include "CommandBulk.h"

PrintVisitor::PrintVisitor(std::ostream &output) :
    m_output(output)
{
}

void PrintVisitor::visitCommand(Command &cmd)
{
    m_output << cmd.value();
}

void PrintVisitor::visitCommandBulk(CommandBulk &bulk)
{
    m_output << "bulk: ";

    bool isFirst = true;
    for (auto &cmd : bulk.commands())
    {
        if (!isFirst)
            m_output << ", ";
        else
            isFirst = false;

        cmd.accept(*this);
    }

    m_output << std::endl;
}
