#include <cassert>
#include <SDL2/SDL_ttf.h>

#include "application.h"
#include "event/input.h"
#include "ecs/entity/entity.h"

InputType Input;

Application* Application::instance = nullptr;
void Application::quit()
{
    assert(instance && "Create Application first!");

    instance->_running = false;
}

Application::Application(const std::string& title, int width, int height) :
    _running(true)
{
    assert(!instance && "Application class instanciated more than once!");
    if (instance)
        log("Application class instanciated more than once!");
    instance = this;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        log("SDL Error : initialisation of subsystems failed!");
        exit(EXIT_FAILURE);
    }
    
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!_window)
    {
        log("SDL Error : unable to create window");
        exit(EXIT_FAILURE);
    }

    renderer.renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_TARGETTEXTURE);
    if (!renderer.renderer)
    {
        log("SDL Error : unable to create a render context");
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() < 0)
    {
        log("SDL Error : unable to initialize TTF library");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Application created" << std::endl;
}

Application::~Application()
{
    SceneManager::clean();
    Entity      ::clean();
    EventManager::clean();
    Renderer    ::clean();

    SDL_DestroyWindow(_window);
    _window = nullptr;

    TTF_Quit();
    SDL_Quit();
}

void Application::log(const std::string& message) const
{
    std::cerr << message << std::endl << SDL_GetError() << std::endl;
}

void Application::run()
{
    while (_running)
    {
        event.handle();

        if (!scene.update())
            quit(); // No more scene left
        else
            scene.render();
            
        renderer.draw();
    }
}
