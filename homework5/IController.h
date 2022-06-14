#pragma once

#include <memory>
#include <string>

class IShape;

class IController
{
public:
    virtual ~IController() {}
    virtual void newDocument() = 0;
    virtual void saveDocument(const std::string &name) = 0;
    virtual void loadDocument(const std::string &name) = 0;
    virtual void addShape(std::unique_ptr<IShape> shape) = 0;
    virtual void removeShape(IShape *shape) = 0;
};
