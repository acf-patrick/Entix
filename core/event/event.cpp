#include "event.h"
#include <cstdlib>
#include <map>
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
        using iter = std::list<Handler>::iterator;

        Event& event = events.front();
        auto tag = event->get<Component::tag>().content;

// perform operations
        for (auto& [id, handler] : handlers[tag])
            handler(*event);

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
            Application::get().quit();
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

void EventManager::connect(const std::string& event_tag, const Handler& handler)
{
    handlers[event_tag].push_back(handler);
}
void EventManager::disconnect(const std::string& event_tag, const std::string& handler_id)
{
    using iter = std::list<Handler>::iterator;
    auto& h = handlers[event_tag];
    
    std::vector<iter> toRemove;
    for (iter i = h.begin(); i != h.end(); ++i)
        if (i->id == handler_id)
            toRemove.push_back(i);
    
    for (auto& it : toRemove)
        h.erase(it);
}

void EventManager::connect(const std::vector<std::string>& tags, const Handler& handler)
{
    for (auto& tag : tags)
        connect(tag, handler);
}
void EventManager::disconnect(const std::vector<std::string>& tags, const std::string& handler_id)
{
    for (auto& tag : tags)
        disconnect(tag, handler_id);
}

EventManager::~EventManager()
{
    handlers.clear();
}
