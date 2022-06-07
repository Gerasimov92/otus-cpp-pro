#pragma once

class Command;
class CommandBulk;

class IVisitor
{
public:
    virtual ~IVisitor() {};
    virtual void visitCommand(Command &cmd) = 0;
    virtual void visitCommandBulk(CommandBulk &bulk) = 0;
};
