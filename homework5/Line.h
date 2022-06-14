#pragma once

#include "IShape.h"
#include "Point.h"

class Line : public IShape
{
public:
    Line(const Point &p1, const Point &p2);

    void draw(IRenderer *renderer) const override;

private:
    Point m_p1;
    Point m_p2;
};
