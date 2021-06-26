#include "event.h"
#include <map>
#include <cstdlib>
#include <ecs/ecs.h>
#include <application/application.h>

EventManager* EventManager::instance = nullptr;
EventManager::EventManager() :
    QUIT                ("SDL quit"),
    KEY_UP              ("SDL key up"),
    KEY_DOWN            ("SDL key down"),
    MOUSE_MOTION        ("SDL mouse motion"),
    MOUSE_BUTTON_UP     ("SDL mouse button up"),
    MOUSE_BUTTON_DOWN   ("SDL mouse button down")
{}

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
        Event& event = events.front();
        auto tag = event->get<Component::tag>().content;

        for (auto l : listners)
        {
            auto& c = l->callbacks;
            if (c.find(tag) != c.end())
                c[tag](*event);
        }

        delete event;
        events.pop();
        bind.erase(tag);
    }
}

void EventManager::SDLEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
        switch (event.type)
        {
        case SDL_QUIT:
            emit(QUIT);
            break;
        case SDL_KEYDOWN:
            emit(KEY_DOWN);
            keys[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            emit(KEY_UP);
            keys[event.key.keysym.scancode] = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            emit(MOUSE_BUTTON_DOWN);
            mouse.pressed = true;
            SDL_GetMouseState(&mouse.x, &mouse.y);
            break;
        case SDL_MOUSEBUTTONUP:
            emit(MOUSE_BUTTON_UP);
            mouse.pressed = true;
            SDL_GetMouseState(&mouse.x, &mouse.y);
            break;
        case SDL_MOUSEMOTION:
            emit(MOUSE_MOTION);
            SDL_GetMouseState(&mouse.x, &mouse.y);
            break;
        default : ;
        }
    
}

Entity& EventManager::emit(const std::string& event_name)
{
    if (bind.find(event_name) != bind.end())
        return *bind[event_name];
        
    Event event = new Entity;
    event->attach<Component::tag>(event_name);

    bind[event_name] = event;
    events.push(event);

    return *event;
}

void EventManager::listnerDestroyed(EventListner* listner)
{
    listners.erase(std::find(listners.begin(), listners.end(), listner));
}