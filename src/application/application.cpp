#include "application.h"

#include <SDL_ttf.h>

#include <cassert>

#include "../ecs/entity/entity.h"
#include "../event/event.h"
#include "../event/input.h"
#include "../scene/scene.h"
#include "../logger/logger.h"

InputType Input;

Application* Application::instance = nullptr;

Application::Application(const std::string& title, int width, int height,
                         SDL_WindowFlags windowFlag)
    : _running(true) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        log("SDL Error : initialisation of subsystems failed!");
        exit(EXIT_FAILURE);
    }

    _window =
        SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, width, height, windowFlag);
    if (!_window) {
        log("SDL Error : unable to create window");
        exit(EXIT_FAILURE);
    }

    SDL_Renderer* r =
        SDL_CreateRenderer(_window, -1, SDL_RENDERER_TARGETTEXTURE);
    if (!r) {
        log("SDL Error : unable to create a render context");
        exit(EXIT_FAILURE);
    }
    // SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

    RenderManager::Get()->renderer = r;
    auto renderer = RenderManager::Get()->renderer;

    if (TTF_Init() < 0) {
        log("SDL Error : unable to initialize TTF library");
        exit(EXIT_FAILURE);
    }

    instance = this;

    Logger::info() << "Application created";
    Logger::endline();
}

Application::~Application() {
    // Make sure to free memory
    Entity::Clean();

    SDL_DestroyWindow(_window);
    _window = nullptr;

    TTF_Quit();
    SDL_Quit();

    Logger::dump("/home/acf-patrick/ecs.log");
}

void Application::log(const std::string& message) const {
    Logger::error() << message << std::endl << SDL_GetError();
    Logger::endline();
}

void Application::run() {
    while (_running) {
        EventManager::Get()->handle();
        auto sceneManager = SceneManager::Get();

        if (!sceneManager->update())
            quit();  // No more scene left
        else
            sceneManager->render();

        RenderManager::Get()->draw();
    }
}

void Application::quit() { _running = false; }

Application& Application::Get() { return *instance; }