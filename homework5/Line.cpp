#include "Line.h"
#include "IRenderer.h"

Line::Line(const Point &p1, const Point &p2) :
    m_p1(p1), m_p2(p2)
{
}

void Line::draw(IRenderer *renderer) const
{
    renderer->drawLine(m_p1, m_p2);
}
