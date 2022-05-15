#include "Observable.h"
#include "IObserver.h"
#include <cassert>

void Observable::attach(const std::shared_ptr<IObserver> &observer)
{
    assert(observer != nullptr);

    for (const auto &o : m_observers)
    {
        if (o.lock() == observer)
            return;
    }

    m_observers.emplace_back(observer);
}

void Observable::detach(const std::shared_ptr<IObserver> &observer)
{
    assert(observer != nullptr);

    for (auto it = m_observers.begin(); it != m_observers.end();)
    {
        if (it->lock() == observer)
            it = m_observers.erase(it);
        else
            ++it;
    }
}

void Observable::notify()
{
    for (auto it = m_observers.begin(); it != m_observers.end();)
    {
        auto ptr = it->lock();
        if (ptr)
        {
            ptr->update();
            ++it;
        }
        else
        {
            it = m_observers.erase(it);
        }
    }
}
