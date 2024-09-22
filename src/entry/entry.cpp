#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include "../application/application.h"
#include "../logger/logger.h"

int main(int argc, char** argv) {
    Logger::info() << "Creating main application";
    Logger::endline();

    std::string configFile((argc > 1) ? argv[1] : "app.cfg");
    auto usingDefaultConfig = false;
    std::ostringstream ss;
    {
        std::ifstream cfg(configFile);
        if (cfg) {
            ss << cfg.rdbuf();
        } else {
            usingDefaultConfig = true;

            Logger::error() << configFile << " was not found";
            Logger::endline();

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

    auto application =
        new Application(title, wSize.x, wSize.y, SDL_WindowFlags(flag));

    auto configPath = std::filesystem::path(configFile).parent_path();
    application->_configPath = configPath.string();
    auto& serializer = application->getSerializer();

    if (node["Position"]) {
        auto n = node["Position"];
        if (n.IsSequence()) {
            auto pos = n.as<VectorI>();
            application->setWindowPosition(pos.x, pos.y);
        }
    }

    if (node["FPS"]) application->setFramerate(node["FPS"].as<int>());

    auto scenesPath = configPath / "scenes";
    if (!std::filesystem::exists(scenesPath) && !usingDefaultConfig) {
        Logger::warn() << "'scenes' folder not found in '" << configPath;
        Logger::endline();
    }

    auto oneSceneDeserialized = false;
    if (node["Scenes"]) {
        for (auto scene : node["Scenes"]) {
            auto scenePath = scenesPath / (scene.as<std::string>() + ".scn");
            if (serializer.deserialize(scenePath.string()))
                oneSceneDeserialized = true;
        }
    }

    if (!oneSceneDeserialized) {
        Logger::error() << "No scene to run";
        Logger::endline();

        delete application;

        return 1;
    }

    application->run();

    Logger::info() << "Exiting application";
    Logger::endline();

    delete application;
    return 0;
}