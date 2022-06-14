#pragma once

#include "IShape.h"
#include "Point.h"

class Rectangle : public IShape
{
public:
    Rectangle(const Point &topLeft, int width, int height);

    void draw(IRenderer *renderer) const override;

private:
    Point m_topLeft;
    Point m_topRight;
    Point m_bottomLeft;
    Point m_bottomRight;
};

