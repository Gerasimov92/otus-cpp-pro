#pragma once

class IRenderer;

class IShape
{
public:
    virtual ~IShape() {};
    virtual void draw(IRenderer *renderer) const = 0;
};
