#include "Rectangle.h"
#include "IRenderer.h"
#include <cassert>

Rectangle::Rectangle(const Point &topLeft, int width, int height) :
    m_topLeft(topLeft),
    m_topRight{topLeft.x + width, topLeft.y},
    m_bottomLeft{topLeft.x, topLeft.y + height},
    m_bottomRight{topLeft.x + width, topLeft.y + height}
{
}

void Rectangle::draw(IRenderer *renderer) const
{
    assert(renderer != nullptr);

    renderer->drawLine(m_topLeft, m_topRight);
    renderer->drawLine(m_topRight, m_bottomRight);
    renderer->drawLine(m_bottomLeft, m_bottomRight);
    renderer->drawLine(m_topLeft, m_bottomLeft);
}
