#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <scene/scene.h>
#include <event/event.h>
#include <renderer/renderer.h>

// interface
class Application
{
public:

    static Application& get();

    virtual ~Application();

    virtual void run();

    void quit();

private:

    bool _running;

protected:

    Application(const std::string&, int, int);


    SceneManager& scene = SceneManager::get();
    EventManager& event = EventManager::get();
    Renderer&  renderer = Renderer::get();

    SDL_Window* _window;

static Application* instance;
};
