#include "application.h"
#include "ecs/entity/entity.h"
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

    Entity::        clean();
    SceneManager::  clean();
    EventManager::  clean();

    SDL_Quit();
}

void Application::run()
{
    SDL_Event sdl_event;

    while (_running)
    {
        event.handle();

        if (!scene.update())
            _running = false; // No more scene left
            
        scene.draw();
    }
}