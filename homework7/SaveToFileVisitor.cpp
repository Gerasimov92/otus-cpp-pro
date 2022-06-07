#include "SaveToFileVisitor.h"
#include "Command.h"
#include "CommandBulk.h"

void SaveToFileVisitor::visitCommand(Command &cmd)
{
    out << cmd.value();
}

void SaveToFileVisitor::visitCommandBulk(CommandBulk &bulk)
{
    if (bulk.commands().empty())
        return;

    out = std::ofstream("bulk" + std::to_string(bulk.commands().front().timestamp()) + ".log");
    if (!out.is_open())
        return;

    out << "bulk: ";

    bool isFirst = true;
    for (auto &cmd : bulk.commands())
    {
        if (!isFirst)
            out << ", ";
        else
            isFirst = false;

        cmd.accept(*this);
    }

    out << std::endl;
    out.close();
}
