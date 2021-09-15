#include "event.h"
#include "input.h"
#include <map>
#include <cstdlib>
#include <ecs/ecs.h>
#include <application/application.h>

EventManagerType::~EventManagerType()
{
    while(!events.empty())
    {
        delete events.front();
        events.pop();
    }
    bind.clear();
    listners.clear();
}

void EventManagerType::handle()
{
    SDLEvents();
    while (!events.empty())
    {
        Event& event = events.front();
        auto tag = event->get<Component::tag>().content;

        for (int i=0; i<(int)listners.size(); ++i)
        {
            auto& l = *listners[i];
            if (l.enabled)
            {
                auto& c = l.callbacks;
                if (c.find(tag) != c.end())
                    c[tag](*event);
            }
        }

        delete event;
        events.pop();
        bind.erase(tag);
    }
}

void EventManagerType::SDLEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        switch (event.type)
        {
        case SDL_QUIT:
            _emit(Input.QUIT);
            break;
        case SDL_KEYDOWN:
            _emit(Input.KEY_DOWN).attachIf<SDL_KeyboardEvent>(event.key);
            Input.keys[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            _emit(Input.KEY_UP).attachIf<SDL_KeyboardEvent>(event.key);
            Input.keys[event.key.keysym.scancode] = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            _emit(Input.MOUSE_BUTTON_DOWN).attachIf<SDL_MouseButtonEvent>(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            _emit(Input.MOUSE_BUTTON_UP).attachIf<SDL_MouseButtonEvent>(event.button);
            break;
        case SDL_MOUSEMOTION:
            _emit(Input.MOUSE_MOTION).attachIf<SDL_MouseMotionEvent>(event.motion);
            break;
        case SDL_MOUSEWHEEL:
            _emit(Input.MOUSE_WHEEL).attachIf<SDL_MouseWheelEvent>(event.wheel);
            break;
        default : ;
        }
}

Entity& EventManagerType::emit(const std::string& event_name)
{
    std::vector<std::string> reserved = {
        Input.QUIT, Input.KEY_DOWN, Input.KEY_UP,
        Input.MOUSE_BUTTON_DOWN, Input.MOUSE_BUTTON_UP,
        Input.MOUSE_WHEEL, Input.MOUSE_MOTION,
        Input.SCENE_LOADED, Input.SCENE_CHANGED
    };
    if (std::find(reserved.begin(), reserved.end(), event_name) != reserved.end())
    {
        std::cerr << "You can not emit this event!" << std::endl;
        if (!invalidEvent)
            invalidEvent = new Entity;
        return *invalidEvent;
    }

    return _emit(event_name);
}

Entity& EventManagerType::_emit(const std::string& event_name)
{
    if (bind.find(event_name) != bind.end())
        return *bind[event_name];
        
    Event event = new Entity;
    event->attach<Component::tag>(event_name);

    bind[event_name] = event;
    events.push(event);

    return *event;
}

void EventManagerType::newListner(EventListner* listner)
{
    listners.push_back(listner);
}

void EventManagerType::listnerDestroyed(EventListner* listner)
{
    listners.erase(std::remove(listners.begin(), listners.end(), listner));
}