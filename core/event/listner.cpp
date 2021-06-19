#include "event.h"

EventListner::EventListner() : manager(EventManager::get())
{
    manager.listners.push_back(this);
}

EventListner::~EventListner()
{
    auto it = manager.listners.begin();
    while (*it != this)
        ++it;
    manager.listners.erase(it);
}

void EventListner::listen(const std::string& event, const Callback& callback)
{
    callbacks[event] = callback;
}

void EventListner::stopListening(const std::string& event)
{
    callbacks.erase(event);
}
