#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <scene/scene.h>

// interface
class Application
{
public:

    virtual ~Application();

    virtual void run() = 0;

protected:

    Application(const std::string&, int, int);

    virtual void update();
    virtual void draw();

    bool _running;

    SceneManager& scene = SceneManager::get();

    SDL_Window* _window;
    SDL_Renderer* _renderer;
};
