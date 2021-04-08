#include "application.h"
#include <cassert>

Application::Application(const std::string& title, int width, int height) :
    _running(true)
{
    assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0 && "SDL Error : initialisation of subsystems failed!");

    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0);
}

Application::~Application()
{
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Application::update()
{}
void Application::draw()
{}
