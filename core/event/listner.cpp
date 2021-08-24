#include "event.h"

EventListner::EventListner() : manager(*EventManager)
{
    manager.listners.push_back(this);
}

EventListner::~EventListner()
{
    manager.listnerDestroyed(this);
}

void EventListner::listen(const std::string& event, const Callback& callback)
{
    callbacks[event] = callback;
}

void EventListner::stopListening(const std::string& event)
{
    callbacks.erase(event);
}

void EventListner::removeCallbacks()
{
    callbacks.clear();
}

void EventListner::enable()
{ 
    enabled = true;    
}

void EventListner::disable()
{
    enabled = false;
}