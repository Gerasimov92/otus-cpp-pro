#pragma once

#include "IShape.h"

#include <list>
#include <memory>

class CompositeShape : public IShape
{
public:
    void draw(IRenderer *renderer) const override;

    void add(std::unique_ptr<IShape> child);
    void remove(IShape *child);

private:
    std::list<std::unique_ptr<IShape>> m_children;
};

