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

    Entity::clean();
    SceneManager::clean();

    SDL_Quit();
}

void Application::run()
{
    SDL_Event event;

    while (_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT or event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_ESCAPE)
                _running = false;
        }

        if (!scene.update())
            _running = false;
            
        scene.draw();
    }
}