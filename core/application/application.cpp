#include "application.h"
#include <cassert>

Application::Application(const std::string& title, int width, int height) :
    _running(true)
{
    assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0 && "SDL Error : initialisation of subsystems failed!");

    SDL_CreateWindowAndRenderer(width, height, 0, &_window, &_renderer);

	assert(_window && "SDL Error : unable to create window!" );
	assert(_renderer && "SDL Error : unable to create a render context!");
}

Application::~Application()
{
	SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    _window = nullptr;
    _renderer = nullptr;
    SDL_Quit();
}

void Application::update()
{}
void Application::draw()
{}
