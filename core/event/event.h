#pragma once

#include <unordered_map>
#include <functional>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <map>

class Entity;
class EventListner;

class EventManager
{
public:
using Event = Entity*;

    static EventManager& get();
    static void clean();

    void handle();

// Entity must have a tag component attached to it
    Entity& emit(const std::string&);

private:
    EventManager() = default;
    
    void SDLEvents();

    std::queue<Event> events;
    std::vector<EventListner*> listners;
    std::unordered_map<std::string, Event> bind;

    static EventManager* instance;

    void listnerDestroyed(EventListner*);

friend class EventListner;
};
/*
namespace Component {
    struct keyboard
    {
        std::map<SDL_Scancode, bool> &content;
    };
}
*/

// Use this class to handle specific event.
class EventListner
{
public:
using Callback = std::function<void(Entity&)>;

    EventListner();
    ~EventListner();

// provide event's tag and function callback
    void listen         (const std::string&, const Callback&);

// stop listening to the event with the given tag
    void stopListening  (const std::string&);

// ignore events
    void removeCallbacks();

private:
    EventManager& manager;
    std::map<std::string, Callback> callbacks;

friend class EventManager;
};