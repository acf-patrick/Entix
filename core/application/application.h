#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <renderer/renderer.h>
#include "../serializer/serializer.h"

// interface
class Application
{
public:
    ~Application();

    void run();

    void quit();
    void log(const std::string&) const;

    static Serializer* serializer;

private:
    Application(const std::string&, int, int, SDL_WindowFlags windowFlag = SDL_WINDOW_SHOWN);

    bool _running = true;
    SDL_Window* _window;

friend int main(int, char**);
};

extern Application* APP;