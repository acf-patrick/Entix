/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Definition of Event listner and Event composer type
 */

#pragma once

#include <SDL.h>

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "input.h"
#include "../manager/manager.h"

class Entity;
class EventListner;

class EventManager: Manager<EventManager> {
   public:
    using Event = Entity*;

    void handle();

    // Emit this message and attach as a tag to an entity
    // Use an EventListner instance to listent to this event anywhere
    Entity& emit(const std::string&);

    static std::shared_ptr<EventManager> Get();

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

    EventManager() = default;
    ~EventManager();

    friend class EventListner;
    friend class Manager<EventManager>;
};

// Use this class to handle specific event.
class EventListner {
   public:
    using Callback = std::function<void(Entity&)>;

    EventListner();
    ~EventListner();

    // provide event's tag and function callback
    EventListner& listen(const std::string&, const Callback&);

    // stop listening to the event with the given tag
    void stopListening(const std::string&);

    // ignore events
    void removeCallbacks();

    // continue listening
    void enable();

    // stop listening temporarily
    void disable();

   private:
    std::shared_ptr<EventManager> manager;
    std::map<std::string, Callback> callbacks;
    bool enabled = true;

    friend class EventManager;
};