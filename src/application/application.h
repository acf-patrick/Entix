/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Define application main class
 */

#pragma once

#include <SDL.h>

#include <filesystem>
#include <memory>
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

    std::filesystem::path getConfigPath();

    template <typename TSerializer>
    void setSerializer() {
        _serializer = std::make_shared<TSerializer>();
    }

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
    std::shared_ptr<Serializer> _serializer;

    static Application* instance;

    friend int main(int, char**);
};

#define HOOK_APPLICATION(CustomHook) \
    ApplicationHook* Application::hook = new CustomHook;

#define RUN_APPLICATION() ApplicationHook* Application::hook = nullptr;