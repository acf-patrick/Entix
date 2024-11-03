/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Define application main class
 */

#pragma once

#include <SDL.h>
#include <SDL2_framerate.h>

#include <filesystem>
#include <memory>
#include <string>

#include "../entry/entry.h"
#include "../ecs/system/system.h"
#include "../renderer/renderer.h"
#include "../serializer/serializer.h"

namespace entix {
namespace core {

class ApplicationHook;

class Application final {
   public:
    ~Application();

    void run();

    void quit();
    void log(const std::string&) const;

    std::filesystem::path getConfigPath();

    // returns FPS
    int getFramerate() const;

    template <typename TSerializer>
    static void setSerializer() {
        _serializer = std::make_shared<TSerializer>();
    }

    template <typename Hook>
    static void setup() {
        _hook = std::make_shared<Hook>();
    }

    Serializer& getSerializer();

    static void Quit();

    static Application& Get();

   private:
    Application(const std::string&, int, int,
                SDL_WindowFlags windowFlag = SDL_WINDOW_SHOWN);

    void setWindowPosition(int, int);

    void setFramerate(unsigned int);

    bool _running = true;
    SDL_Window* _window;
    std::string _configPath;
    FPSmanager _fpsManager;
    std::vector<int> _framerates;
    int _framerate = 0;
    const int _rateSamples = 10;

    static std::shared_ptr<Serializer> _serializer;

    static std::shared_ptr<ApplicationHook> _hook;
    
    static Application* instance;

    friend int entix::main(int, char**);
};

}   // namespace core
}   // namespace entix