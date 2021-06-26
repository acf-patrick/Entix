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

    const std::string QUIT;
    const std::string KEY_UP;
    const std::string KEY_DOWN;
    const std::string MOUSE_MOTION;
    const std::string MOUSE_BUTTON_UP;
    const std::string MOUSE_BUTTON_DOWN;

    static EventManager& get();
    static void clean();

    void handle();

// Entity must have a tag component attached to it
    Entity& emit(const std::string&);

private:

    EventManager();
    void SDLEvents();

    std::queue<Event> events;
    std::vector<EventListner*> listners;
    std::unordered_map<std::string, Event> bind;

    static EventManager* instance;

    void listnerDestroyed(EventListner*);

public:

    struct Mouse
    {
        bool pressed;
        int x, y;
    };
    Mouse mouse;

    std::map<SDL_Scancode, bool> keys;

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

// Use this class to handle specific event by providing its tag
// and provide a callback function
class EventListner
{
public:
using Callback = std::function<void(Entity&)>;

    EventListner();
    ~EventListner();

    void listen         (const std::string&, const Callback&);
    void stopListening  (const std::string&);

private:
    EventManager& manager;
    std::map<std::string, Callback> callbacks;

friend class EventManager;
};