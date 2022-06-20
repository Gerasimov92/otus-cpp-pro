#pragma once

#include "IVisitor.h"
#include <ostream>

class PrintVisitor : public IVisitor
{
public:
    PrintVisitor(std::ostream &output);
    void visitCommand(Command &cmd) override;
    void visitCommandBulk(CommandBulk &bulk) override;

private:
    std::ostream &m_output;
};
