#pragma once

class Point;

class IRenderer
{
public:
    virtual ~IRenderer() {}
    virtual void begin() = 0;
    virtual void end() = 0;
    virtual void drawLine(const Point &p1, const Point &p2) = 0;
};
