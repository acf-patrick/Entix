#include "event.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <map>

const std::string EventManager::quit = "SDL QUIT";

EventManager* EventManager::instance = nullptr;

EventManager& EventManager::get()
{
    if (!instance)
        instance = new EventManager;

    return *instance;
}
void EventManager::clean()
{
    delete instance;
    instance = nullptr;
}

void EventManager::handle()
{
    SDLEvents();
    while (!events.empty())
    {
        using iter = std::list<Handler>::iterator;

        Event event = events.front();
        auto tag = event->get<Component::tag>().content;

// perform operations
        {
            std::vector<iter> to_remove;

            for (iter i = handlers[tag].begin(); i != handlers[tag].end(); ++i)
            {
                auto& handler = *i;
                handler.func(*event);
                if (!handler.persist)
                    to_remove.push_back(i);
            }

            for (auto& r : to_remove)
                handlers[tag].erase(r);
        }

        events.pop();
    }
}

void EventManager::SDLEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            emit(quit);
            break;
        default : ;
        }
    }
}

Entity& EventManager::emit(const std::string& event_name)
{
    if (bind.find(event_name) != bind.end())
        return *bind[event_name];
        
    Event event = new Entity;
    event->attach<Component::tag>(event_name);
    _cache.push_back(event);

    bind[event_name] = event;
    events.push(event);

    return *event;
}

void EventManager::connect(const std::string& event_tag, _handler handler, bool once)
{
    Handler h = { !once, handler };
    handlers[event_tag].push_back(h);
}

EventManager::~EventManager()
{
    handlers.clear();

    for (auto& event : _cache)
    {
        delete event;
        event = nullptr;
    }
    _cache.clear();
}
