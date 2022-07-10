#pragma once

#include "IVisitor.h"
#include <fstream>
#include <string>

class SaveToFileVisitor : public IVisitor
{
public:
    SaveToFileVisitor(const std::string &postfix = "");
    void visitCommand(Command &cmd) override;
    void visitCommandBulk(CommandBulk &bulk) override;

private:
    std::ofstream m_out;
    std::string m_postfix;
};
