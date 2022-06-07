#pragma once

class IVisitor;

class IVisitable
{
public:
    virtual ~IVisitable() {};
    virtual void accept(IVisitor &visitor) = 0;
};

