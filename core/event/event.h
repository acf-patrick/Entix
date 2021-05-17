#pragma once

#include <unordered_map>
#include <functional>
#include <ecs/ecs.h>
#include <vector>
#include <string>
#include <queue>
#include <list>

class EventManager
{
public:

    static const std::string quit;

using _handler  = std::function<void(Entity&)>;
using Event     = Entity;

    struct Handler
    {
        // Always execute the instruction
        bool persist; 
        _handler func;
    };

public:

    static EventManager& get();
    static void clean();

    void handle();

// Entity must have a tag component attached to it
    Entity& emit(const std::string&);

    void connect(const std::string&, _handler, bool once = true);

private:

    void SDLEvents();

    std::queue<Event> events;
    std::unordered_map<std::string, Event> bind;
    std::unordered_map<std::string, std::list<Handler>> handlers;


    std::vector<Event*> _cache;

    static EventManager* instance;
};