#include "Document.h"
#include "CompositeShape.h"

Document::Document() :
    m_root(std::make_unique<CompositeShape>())
{
}

void Document::draw(IRenderer *renderer) const
{
    m_root->draw(renderer);
}

void Document::clear()
{
    m_root.reset(new CompositeShape);
    notify();
}

void Document::add(std::unique_ptr<IShape> shape)
{
    m_root->add(std::move(shape));
    notify();
}

void Document::remove(IShape *shape)
{
    m_root->remove(shape);
    notify();
}
