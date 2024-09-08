#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include "../logger/logger.h"
#include "../application/application.h"

int main(int argc, char **argv) {
    Logger::info() << "Creating main application";
    Logger::endline();

    std::string configFile((argc > 1) ? argv[1] : "app.cfg");
    std::ostringstream ss;
    {
        std::ifstream cfg(configFile);
        if (cfg) {
            ss << cfg.rdbuf();
        } else {
            std::cerr << configFile << " was not found" << std::endl;
            
            Logger::info() << "Default configuration used";
            Logger::endline();
        }
    }

    YAML::Node node = YAML::Load(ss.str());
    std::string title = "Untitled";
    if (node["Title"]) title = node["Title"].as<std::string>();

    VectorI wSize(600, 400);
    if (node["Size"]) wSize = node["Size"].as<VectorI>();

    int flag(0);
    if (node["Flags"]) {
        std::map<std::string, SDL_WindowFlags> bind = {
            {"shown", SDL_WINDOW_SHOWN},
            {"resizable", SDL_WINDOW_RESIZABLE},
            {"minimized", SDL_WINDOW_MINIMIZED},
            {"maximized", SDL_WINDOW_MAXIMIZED},
            {"borderless", SDL_WINDOW_BORDERLESS},
            {"always on top", SDL_WINDOW_ALWAYS_ON_TOP},
            {"fullscreen", SDL_WINDOW_FULLSCREEN_DESKTOP}};
        for (auto f : node["Flags"]) flag |= bind[f.as<std::string>()];
    }

    auto APP = new Application(title, wSize.x, wSize.y, SDL_WindowFlags(flag));
    assert(APP->serializer &&
           "No serializer declared. Create serializer in global scope!");
    auto &s = *APP->serializer;

    if (node["Position"]) {
        auto n = node["Position"];
        if (n.IsSequence()) {
            auto pos = n.as<VectorI>();
            SDL_SetWindowPosition(APP->_window, pos.x, pos.y);
        }
    }

    if (node["Scenes"]) {
        using Path = std::filesystem::path;
        Path configPath(configFile);

        // configPath.parent_path()
        for (auto scene : node["Scenes"]) {
            Path scenePath("scenes");
            scenePath /= scene.as<std::string>() + ".scn";
            scenePath = configPath.parent_path() / scenePath;

            s.deserialize(scenePath.string());
        }
    }

    APP->run();

    Logger::info() << "Exiting application";
    Logger::endline();
    
    delete APP;
    return 0;
}