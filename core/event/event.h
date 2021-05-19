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

    static const std::string quit;
    static const std::string keyup;
    static const std::string keydown;

    using Event     = Entity*;
    using _handler  = std::function<void(Entity&)>;

    struct Handler
    {
        // execute this instruction count times
        int count;
        _handler func;
    };

public:

    static EventManager& get();
    static void clean();

    void handle();

// Entity must have a tag component attached to it
    Entity& emit(const std::string&);

    void connect(const std::string&, _handler, int count = -1);
    void connect(const std::vector<std::string>&, _handler, int count = -1);

private:

    EventManager() = default;
    ~EventManager();

    void SDLEvents();

    std::queue<Event> events;
    std::unordered_map<std::string, Event> bind;
    std::unordered_map<std::string, std::list<Handler>> handlers;


    // std::vector<Event> _cache;

    static EventManager* instance;

public:

    std::map<SDL_Scancode, bool> keys;
};

namespace Component {
    struct keyboard
    {
        std::map<SDL_Scancode, bool> &content;
    };
}