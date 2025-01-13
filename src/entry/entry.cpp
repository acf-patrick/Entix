#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#if true || defined(NDEBUG)
#include <app_config.h>
#endif

#include "../application/application.h"
#include "../logger/logger.h"

namespace entix {

int main(int argc, char** argv) {
    Logger::info() << "Creating main application";
    Logger::endline();

    auto usingDefaultConfig = false;

#if true || defined(NDEBUG)
    if (g_app_config_len == 0) {
        Logger::error() << "Application config not found";
        Logger::endline();

        return -1;
    }

    const std::string yaml(g_app_config[0]);
    const auto execPath = std::filesystem::canonical(argv[0]);
    const auto configPath = execPath.parent_path();
#else
    std::string configFile((argc > 1) ? argv[1] : "app.cfg");
    std::ostringstream ss;
    {
        std::ifstream cfg(configFile);
        if (cfg) {
            ss << cfg.rdbuf();
        } else {
            usingDefaultConfig = true;

            Logger::error()
                << "Executable running in debug mode : " << configFile
                << " was not found";
            Logger::endline();

            Logger::info() << "Default configuration used";
            Logger::endline();
        }
    }

    const auto yaml = ss.str();
    const auto configPath = std::filesystem::path(configFile).parent_path();
#endif

    YAML::Node node = YAML::Load(yaml);
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

    core::Application application(title, wSize.x, wSize.y,
                                  SDL_WindowFlags(flag));
    application._configPath = configPath.string();

    auto& serializer = application.getSerializer();

    if (node["Position"]) {
        auto n = node["Position"];
        if (n.IsSequence()) {
            auto pos = n.as<VectorI>();
            application.setWindowPosition(pos.x, pos.y);
        }
    }

    if (node["FPS"]) application.setFramerate(node["FPS"].as<int>());

#if false && !defined(NDEBUG)
    auto scenesPath = configPath / core::Scene::FOLDER;
    if (!std::filesystem::exists(scenesPath) && !usingDefaultConfig) {
        Logger::warn() << "'scenes' folder not found in '" << configPath;
        Logger::endline();
    }
#endif

    auto oneSceneDeserialized = false;
    if (node["Scenes"]) {
        auto scenes = node["Scenes"].as<std::vector<std::string>>();
        if (!scenes.empty()) {
            core::SceneManager::Get()->setListOfScenes(scenes);
            oneSceneDeserialized = true;
        }
    }

    if (!oneSceneDeserialized) {
        Logger::error() << "No scene to run";
        Logger::endline();

        return 1;
    }

    application.run();

    Logger::info() << "Exiting application";
    Logger::endline();

    return 0;
}

}  // namespace entix