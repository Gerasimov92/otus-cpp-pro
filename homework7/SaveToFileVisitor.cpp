#include "SaveToFileVisitor.h"
#include "Command.h"
#include "CommandBulk.h"

SaveToFileVisitor::SaveToFileVisitor(const std::string &postfix) :
    m_postfix(postfix)
{
}

void SaveToFileVisitor::visitCommand(Command &cmd)
{
    m_out << cmd.value();
}

void SaveToFileVisitor::visitCommandBulk(CommandBulk &bulk)
{
    if (bulk.commands().empty())
        return;

    std::string filename = "bulk" + std::to_string(bulk.commands().front().timestamp());
    if (!m_postfix.empty())
        filename += "_" + m_postfix;
    filename += ".log";

    m_out = std::ofstream(filename);
    if (!m_out.is_open())
        return;

    m_out << "bulk: ";

    bool isFirst = true;
    for (auto &cmd : bulk.commands())
    {
        if (!isFirst)
            m_out << ", ";
        else
            isFirst = false;

        cmd.accept(*this);
    }

    m_out << std::endl;
    m_out.close();
}
