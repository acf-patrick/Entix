#include <cassert>
#include <SDL2/SDL_ttf.h>

#include "application.h"
#include "event/input.h"
#include "event/event.h"
#include "scene/scene.h"
#include "ecs/entity/entity.h"

InputType Input;
EventManagerType* EventManager;
SceneManagerType* SceneManager;
RenderManager* Renderer;

Application::Application(const std::string& title, int width, int height, SDL_WindowFlags windowFlag) :
    _running(true)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        log("SDL Error : initialisation of subsystems failed!");
        exit(EXIT_FAILURE);
    }
    
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlag);
    if (!_window)
    {
        log("SDL Error : unable to create window");
        exit(EXIT_FAILURE);
    }

    // Initializing managers
    Renderer = new RenderManager;
    EventManager = new EventManagerType;
    SceneManager = new SceneManagerType;

    SDL_Renderer* r = SDL_CreateRenderer(_window, -1, SDL_RENDERER_TARGETTEXTURE);
    if (!r)
    {
        log("SDL Error : unable to create a render context");
        exit(EXIT_FAILURE);
    }
    // SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    Renderer->renderer = r;

    if (TTF_Init() < 0)
    {
        log("SDL Error : unable to initialize TTF library");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Application created" << std::endl;
}

Application::~Application()
{
    delete SceneManager;
    delete EventManager;
    delete Renderer;
    Entity::clean();

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
        EventManager->handle();

        if (!SceneManager->update())
            quit(); // No more scene left
        else
            SceneManager->render();
            
        Renderer->draw();
    }
}

void Application::quit()
{
    _running = false;
}
