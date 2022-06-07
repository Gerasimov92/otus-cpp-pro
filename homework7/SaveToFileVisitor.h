#pragma once

#include "IVisitor.h"
#include <fstream>

class SaveToFileVisitor : public IVisitor
{
public:
    void visitCommand(Command &cmd) override;
    void visitCommandBulk(CommandBulk &bulk) override;

private:
    std::ofstream out;
};
