#pragma once

#include "IObserver.h"
#include "IShape.h"
#include <memory>

class IView : public IObserver
{
public:
    virtual ~IView() {}
    virtual void onNewDocumentEvent() = 0;
    virtual void onSaveDocumentEvent(const std::string &name) = 0;
    virtual void onLoadDocumentEvent(const std::string &name) = 0;
    virtual void onAddShapeEvent(std::unique_ptr<IShape> shape) = 0;
    virtual void onRemoveShapeEvent(IShape *shape) = 0;
};
