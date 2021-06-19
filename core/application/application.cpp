#include "application.h"
#include "ecs/entity/entity.h"
#include <cassert>

Application* Application::instance = nullptr;
Application& Application::get()
{
    assert(instance && "Create Application first!");

    return *instance;
}

Application::Application(const std::string& title, int width, int height) :
    _running(true)
{
    assert(!instance && "Application class instanciated more than once!");

    instance = this;

    assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0 && "SDL Error : initialisation of subsystems failed!");

    SDL_CreateWindowAndRenderer(width, height, 0, &_window, &renderer.renderer);

	assert(_window && "SDL Error : unable to create window!" );
	assert(renderer.renderer && "SDL Error : unable to create a render context!");

}

Application::~Application()
{
    EventManager::  clean();
    SceneManager::  clean();
    Entity      ::  clean();
    Renderer    ::  clean();

    SDL_DestroyWindow(_window);
    _window = nullptr;

    SDL_Quit();

    std::cout << "Application exited\n";
}

void Application::run()
{
    while (_running)
    {
        event.handle();

        if (!scene.update())
            quit(); // No more scene left
            
        renderer.draw();
        renderer.update();
    }
}

void Application::quit()
{
    _running = false;
}