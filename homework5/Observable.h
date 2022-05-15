#pragma once

#include <list>
#include <memory>

class IObserver;

class Observable
{
public:
    void attach(const std::shared_ptr<IObserver> &observer);
    void detach(const std::shared_ptr<IObserver> &observer);
    void notify();

private:
    std::list<std::weak_ptr<IObserver>> m_observers;
};
