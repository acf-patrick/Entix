#pragma once

#include <unordered_map>
#include <functional>
#include <ecs/ecs.h>
#include <vector>
#include <string>
#include <queue>

class EventManager
{
public:

using Handler   = std::function<void(Entity)>;
using Event     = Entity;

    static EventManager& get();
    static void clean();

    void handle();

// Entity must have a tag component attached to it
    void emit(Event);

    void connect(const std::string&, Handler);

private:

    std::queue<Event> events;
    std::unordered_map<std::string, Event> bind;
    std::unordered_map<EntityID, std::vector<Handler>> handlers;

    static EventManager* instance;
};