#include "event.h"
#include "input.h"
#include <map>
#include <cstdlib>
#include <ecs/ecs.h>
#include <application/application.h>

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
            emit(Input.QUIT);
            break;
        case SDL_KEYDOWN:
            emit(Input.KEY_DOWN);
            Input.keys[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            emit(Input.KEY_UP);
            Input.keys[event.key.keysym.scancode] = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            emit(Input.MOUSE_BUTTON_DOWN);
            Input.mouse.pressed = true;
            SDL_GetMouseState(&Input.mouse.x, &Input.mouse.y);
            break;
        case SDL_MOUSEBUTTONUP:
            emit(Input.MOUSE_BUTTON_UP);
            Input.mouse.pressed = true;
            SDL_GetMouseState(&Input.mouse.x, &Input.mouse.y);
            break;
        case SDL_MOUSEMOTION:
            emit(Input.MOUSE_MOTION);
            SDL_GetMouseState(&Input.mouse.x, &Input.mouse.y);
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