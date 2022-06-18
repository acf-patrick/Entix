/**
 * @author acf-patrick (miharisoap@gmail.com)
 * 
 * Definition of Event listner and Event composer type
 */

#pragma once

#include <unordered_map>
#include <functional>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <map>

#include "input.h"

class Application;

class Entity;
class EventListner;

class EventManagerType
{
public:
    using Event = Entity*;

    void handle();
    
    // Emit this message and attach as a tag to an entity
    // Use an EventListner instance to listent to this event anywhere
    Entity& emit(const std::string&);

private:
    
    Event invalidEvent = nullptr;

    Entity& _emit(const std::string&);
    void SDLEvents();

    std::queue<Event> events;
    std::vector<EventListner*> listners;
    std::unordered_map<std::string, Event> bind;

    // std::vector<EventListner*> toAdd;
    // std::vector<EventListner*> toRemove;

    void listnerDestroyed(EventListner*);
    void newListner(EventListner*);

    EventManagerType() = default;
    ~EventManagerType();

friend class EventListner;
friend class Application;
};

// Use this class to handle specific event.
class EventListner
{
public:
using Callback = std::function<void(Entity&)>;

    EventListner();
    ~EventListner();

    // provide event's tag and function callback
    void listen(const std::string&, const Callback&);

    // stop listening to the event with the given tag
    void stopListening(const std::string&);

    // ignore events
    void removeCallbacks();

    // continue listening
    void enable();

    // stop listening temporarily
    void disable();

private:
    EventManagerType& manager;
    std::map<std::string, Callback> callbacks;
    bool enabled = true;

friend class EventManagerType;
};

extern EventManagerType* EventManager;