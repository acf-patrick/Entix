#include "event.h"
#include <cstdlib>

EventManager* EventManager::instance = nullptr;

EventManager& EventManager::get()
{
    if (!instance)
        instance = (EventManager*)malloc(sizeof (EventManager));

    return *instance;
}
void EventManager::clean()
{
    free(instance);

    instance = nullptr;
}

void EventManager::handle()
{
    while (!events.empty())
    {
        Event& event = events.front();
        for (auto& handler : handlers[event])
            handler(event);
        events.pop();
    }
}

void EventManager::emit(Event entity)
{ 
    // Entity* entity = Entity::get(event);

    assert (entity.has<Component::tag>() && "Entity of type Event must have a tag component");

    bind[entity.get<Component::tag>().content] = entity;
    events.push(entity);
}

void EventManager::connect(const std::string& event_tag, Handler handler)
{
    handlers[bind[event_tag]].push_back(handler);
}