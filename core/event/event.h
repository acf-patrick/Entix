#pragma once

#include <unordered_map>
#include <functional>
#include <SDL2/SDL.h>
#include <ecs/ecs.h>
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <map>

class EventManager
{
public:
    const std::string QUIT;
    const std::string KEY_UP;
    const std::string KEY_DOWN;
    const std::string MOUSE_MOTION;
    const std::string MOUSE_BUTTON_UP;
    const std::string MOUSE_BUTTON_DOWN;

    using Event     = Entity*;
    using _handler  = std::function<void(Entity&)>;

    struct Handler
    {
        std::string id; // allow us to disconnect event-function later
        _handler func;
    };

public:
    static EventManager& get();
    static void clean();

    void handle();

// Entity must have a tag component attached to it
    Entity& emit(const std::string&);

    void connect(const std::string&, const Handler&);
    void connect(const std::vector<std::string>&, const Handler&);

    void disconnect(const std::string&, const std::string&);
    void disconnect(const std::vector<std::string>&, const std::string&);

private:
    EventManager();
    ~EventManager();

    void SDLEvents();

    std::queue<Event> events;
    std::unordered_map<std::string, Event> bind;
    std::unordered_map<std::string, std::list<Handler>> handlers;

    static EventManager* instance;

public:

    struct Mouse
    {
        bool pressed;
        int x, y;
    };
    Mouse mouse;

    std::map<SDL_Scancode, bool> keys;
};
/*
namespace Component {
    struct keyboard
    {
        std::map<SDL_Scancode, bool> &content;
    };
}
*/