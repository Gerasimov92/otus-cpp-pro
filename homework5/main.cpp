#include <cassert>
#include <iostream>

#include "IController.h"
#include "IRenderer.h"
#include "IView.h"

#include "CompositeShape.h"
#include "Document.h"
#include "Line.h"
#include "Observable.h"
#include "Point.h"
#include "Rectangle.h"

std::ostream &operator<<(std::ostream &out, const Point &p)
{
    return out << '(' << p.x << ',' << p.y << ')';
}

class ConsoleRenderer : public IRenderer
{
public:
    void begin() override
    {
        std::cout << "Begin rendering" << std::endl;
    }

    void end() override
    {
        std::cout << "End rendering" << std::endl;
    }

    void drawLine(const Point &p1, const Point &p2) override
    {
        std::cout << "Draw line: " << p1 << ' ' << p2 << std::endl;
    }
};

class ConsoleView : public IView, public std::enable_shared_from_this<ConsoleView>
{
public:
    static std::shared_ptr<ConsoleView> create(const std::shared_ptr<Document> &doc, const std::shared_ptr<IController> &controller)
    {
        assert(doc != nullptr);
        assert(controller != nullptr);

        auto view = std::shared_ptr<ConsoleView>(new ConsoleView(doc, controller));
        doc->attach(view);
        return view;
    }

    void update() override
    {
        m_renderer.begin();
        m_doc->draw(&m_renderer);
        m_renderer.end();
    }

    void onNewDocumentEvent() override
    {
        m_controller->newDocument();
    }

    void onSaveDocumentEvent(const std::string &name) override
    {
        m_controller->saveDocument(name);
    }

    void onLoadDocumentEvent(const std::string &name) override
    {
        m_controller->loadDocument(name);
    }

    void onAddShapeEvent(std::unique_ptr<IShape> shape) override
    {
        m_controller->addShape(std::move(shape));
    }

    void onRemoveShapeEvent(IShape *shape) override
    {
        m_controller->removeShape(shape);
    }

private:
    ConsoleView(const std::shared_ptr<Document> &doc, const std::shared_ptr<IController> &controller) :
        m_doc(doc), m_controller(controller)
    {
    }

    ConsoleRenderer m_renderer;
    std::shared_ptr<Document> m_doc;
    std::shared_ptr<IController> m_controller;
};

class TestController : public IController
{
public:
    TestController(const std::shared_ptr<Document> &doc) :
        m_doc(doc)
    {
    }

    void newDocument() override
    {
        m_doc->clear();
    }

    void saveDocument(const std::string &name) override
    {
        std::cout << "Save document with name: " << name << std::endl;
    }

    void loadDocument(const std::string &name) override
    {
        std::cout << "Load document with name: " << name << std::endl;
    }

    void addShape(std::unique_ptr<IShape> shape) override
    {
        m_doc->add(std::move(shape));
    }

    void removeShape(IShape *shape) override
    {
        m_doc->remove(shape);
    }

private:
    std::shared_ptr<Document> m_doc;
};

int main()
{
    auto doc = std::make_shared<Document>();
    auto controller = std::make_shared<TestController>(doc);
    auto view = ConsoleView::create(doc, controller);

    view->onNewDocumentEvent();

    auto compositeShape = std::make_unique<CompositeShape>();
    auto line = std::make_unique<Line>(Point{0, 0}, Point{10, 10});
    auto *ptr = line.get();
    compositeShape->add(std::move(line));
    view->onAddShapeEvent(std::move(compositeShape));

    auto rect = std::make_unique<Rectangle>(Point{10, 10}, 20, 30);
    view->onAddShapeEvent(std::move(rect));

    view->onRemoveShapeEvent(ptr);

    view->onSaveDocumentEvent("TestDoc");
    view->onLoadDocumentEvent("TestDoc");

    return 0;
}
