/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Define application main class
 */

#pragma once

#include <SDL.h>

#include <filesystem>
#include <string>

#include "../renderer/renderer.h"
#include "../serializer/serializer.h"

#define USE_SERIALIZER(CustomSerializer) \
    Serializer* Application::serializer = new CustomSerializer;

class Application final {
   public:
    ~Application();

    void run();

    void quit();
    void log(const std::string&) const;

    static Application& Get();

    static Serializer* serializer;

    static std::filesystem::path GetConfigPath();

   private:
    Application(const std::string&, int, int,
                SDL_WindowFlags windowFlag = SDL_WINDOW_SHOWN);

    void setWindowPosition(int, int);

    bool _running = true;
    SDL_Window* _window;

    static Application* instance;
    static std::string configPath;

    friend int main(int, char**);
};
