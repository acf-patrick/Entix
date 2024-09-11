/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Define application main class
 */

#pragma once

#include <SDL.h>

#include <filesystem>
#include <string>

#include "../ecs/system/system.h"
#include "../renderer/renderer.h"
#include "../serializer/serializer.h"

class ApplicationHook;

class Application final {
   public:
    ~Application();

    void run();

    void quit();
    void log(const std::string&) const;

    template <typename TSerializer>
    void setSerializer() {
        _serializer = new TSerializer;
    }

    template <typename TSystem>
    void addSystem() {
        auto manager = SystemManager::Get();
        manager->addSystem<TSystem>();
    }

    std::filesystem::path getConfigPath();

    Serializer& getSerializer();

    static Application& Get();

    static ApplicationHook* hook;

   private:
    Application(const std::string&, int, int,
                SDL_WindowFlags windowFlag = SDL_WINDOW_SHOWN);

    void setWindowPosition(int, int);

    bool _running = true;
    SDL_Window* _window;
    std::string _configPath;
    Serializer* _serializer = nullptr;

    static Application* instance;

    friend int main(int, char**);
};

#define HOOK_APPLICATION(CustomHook) \
    ApplicationHook* Application::hook = new CustomHook;

#define RUN_APPLICATION ApplicationHook* Application::hook = nullptr;