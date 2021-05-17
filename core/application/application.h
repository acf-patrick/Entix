#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <scene/scene.h>
#include <event/event.h>

// interface
class Application
{
public:

    virtual ~Application();

    virtual void run();

protected:

    Application(const std::string&, int, int);

    bool _running;

    SceneManager& scene = SceneManager::get();
    EventManager& event = EventManager::get();

    SDL_Window* _window;
    SDL_Renderer* _renderer;
};
