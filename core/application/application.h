#pragma once

#include <string>
#include <SDL2/SDL.h>

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

    SDL_Window* _window;
    SDL_Renderer* _renderer;
};
