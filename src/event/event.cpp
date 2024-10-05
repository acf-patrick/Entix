#include "event.h"

#include <cstdlib>
#include <map>

#include "../application/application.h"
#include "../ecs/ecs.h"
#include "../scene/scene.h"
#include "./input.h"

EventManager::~EventManager() {
    while (!_events.empty()) {
        delete _events.front();
        _events.pop();
    }

    _bind.clear();
    _listners.clear();
}

void EventManager::handle() {
    SDLEvents();
    while (!_events.empty()) {
        Event& event = _events.front();
        auto tag = event->get<Component::tag>().content;

        for (int i = 0; i < (int)_listners.size(); ++i) {
            auto& listener = *_listners[i];
            if (listener.enabled) {
                auto& callbacks = listener.callbacks;
                if (callbacks.find(tag) != callbacks.end()) {
                    auto& callback = callbacks[tag];
                    if (callback.withParameter) {
                        auto& function = *callback.function;
                        function(*event);
                    } else {
                        auto& function = *callback.noParamFunction;
                        function();
                    }
                }
            }
        }

        delete event;
        _events.pop();
        _bind.erase(tag);
    }
}

void EventManager::SDLEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _emit(Input::Event::QUIT);
                break;
            case SDL_KEYDOWN:
                _emit(Input::Event::KEY_DOWN)
                    .attachIf<SDL_KeyboardEvent>(event.key);
                Input::_keys[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                _emit(Input::Event::KEY_UP)
                    .attachIf<SDL_KeyboardEvent>(event.key);
                Input::_keys[event.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                _emit(Input::Event::MOUSE_BUTTON_DOWN)
                    .attachIf<SDL_MouseButtonEvent>(event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _emit(Input::Event::MOUSE_BUTTON_UP)
                    .attachIf<SDL_MouseButtonEvent>(event.button);
                break;
            case SDL_MOUSEMOTION:
                _emit(Input::Event::MOUSE_MOTION)
                    .attachIf<SDL_MouseMotionEvent>(event.motion);
                break;
            case SDL_MOUSEWHEEL:
                _emit(Input::Event::MOUSE_WHEEL)
                    .attachIf<SDL_MouseWheelEvent>(event.wheel);
                break;
            default:;
        }
    }
}

Entity& EventManager::emit(const std::string& event_name) {
    std::vector<std::string> reserved = {
        Input::Event::QUIT,
        Input::Event::KEY_DOWN,
        Input::Event::KEY_UP,
        Input::Event::MOUSE_BUTTON_DOWN,
        Input::Event::MOUSE_BUTTON_UP,
        Input::Event::MOUSE_WHEEL,
        Input::Event::MOUSE_MOTION,
        Scene::Event::LOADED,
        Scene::Event::CHANGED,
        SystemManager::Event::SYSTEM_ACTIVATED,
        SystemManager::Event::SYSTEM_DEACTIVATED,
    };

    auto invalidEvent = std::find(reserved.begin(), reserved.end(),
                                  event_name) != reserved.end();
    auto message = "You can not emit event : " + event_name;
    assert(invalidEvent && message.c_str());

    return _emit(event_name);
}

Entity& EventManager::_emit(const std::string& event_name) {
    if (_bind.find(event_name) != _bind.end()) return *_bind[event_name];

    Event event = new Entity;
    event->attach<Component::tag>(event_name);

    _bind[event_name] = event;
    _events.push(event);

    std::vector<std::string> ignored = {
        Input::Event::QUIT,
        Input::Event::KEY_DOWN,
        Input::Event::KEY_UP,
        Input::Event::MOUSE_BUTTON_DOWN,
        Input::Event::MOUSE_BUTTON_UP,
        Input::Event::MOUSE_WHEEL,
        Input::Event::MOUSE_MOTION,
    };

    if (std::find(ignored.begin(), ignored.end(), event_name) == ignored.end()) {
        Logger::info("Event") << event_name;
        Logger::endline();
    }

    return *event;
}

void EventManager::newListner(EventListner* listner) {
    _listners.push_back(listner);
}

void EventManager::listnerDestroyed(EventListner* listner) {
    _listners.erase(std::remove(_listners.begin(), _listners.end(), listner));
}

// static
std::shared_ptr<EventManager> EventManager::Get() { return createInstance(); }