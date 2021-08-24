#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <renderer/renderer.h>

// interface
class Application
{
public:
    virtual ~Application();

    virtual void run();

    void quit();
    void log(const std::string&) const;

private:
    bool _running = true;

protected:

    Application(const std::string&, int, int);

    SDL_Window* _window;

friend int main(int, char**);
};

extern Application* APP;