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
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "../manager/manager.h"
#include "input.h"

namespace entix {

namespace ecs {
class Entity;
}

namespace core {

class EventListner;

class EventManager : Manager<EventManager> {
   public:
    using Event = ecs::Entity*;

    void handle();

    // Emit this message and attach as a tag to an Entity
    // Use an EventListner instance to listent to this event anywhere
    ecs::Entity& emit(const std::string&);

    static std::shared_ptr<EventManager> Get();

   private:
    ecs::Entity& _emit(const std::string&);
    void SDLEvents();

    std::queue<Event> _events;
    std::vector<EventListner*> _listners;
    std::unordered_map<std::string, Event> _bind;

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
    using WithParameter = std::function<void(ecs::Entity&)>;
    using WithoutParameter = std::function<void()>;

    struct Callback {
        bool withParameter = true;
        std::optional<WithParameter> function;
        std::optional<WithoutParameter> noParamFunction;
    };

   public:
    EventListner();
    ~EventListner();

    // provide event's tag and function callback
    EventListner& listen(const std::string&, const WithParameter&);

    // provide event's tag and function callback
    EventListner& listen(const std::string&, const WithoutParameter&);

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

}  // namespace core
}  // namespace entix
