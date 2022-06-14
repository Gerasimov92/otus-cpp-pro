#pragma once

#include "Observable.h"
#include <memory>

class CompositeShape;
class IRenderer;
class IShape;

class Document : public Observable
{
public:
    Document();
    void draw(IRenderer *renderer) const;
    void clear();
    void add(std::unique_ptr<IShape> shape);
    void remove(IShape *shape);

private:
    std::unique_ptr<CompositeShape> m_root;
};

