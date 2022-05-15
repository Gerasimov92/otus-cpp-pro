#include "CompositeShape.h"
#include <cassert>

void CompositeShape::draw(IRenderer *renderer) const
{
    assert(renderer != nullptr);

    for (const auto &child : m_children)
        child->draw(renderer);
}

void CompositeShape::add(std::unique_ptr<IShape> child)
{
    assert(child != nullptr);
    m_children.emplace_back(std::move(child));
}

void CompositeShape::remove(IShape *child)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if (it->get() == child)
        {
            m_children.erase(it);
            break;
        }

        auto *composite = dynamic_cast<CompositeShape*>(it->get());
        if (composite)
            composite->remove(child);
    }
}
