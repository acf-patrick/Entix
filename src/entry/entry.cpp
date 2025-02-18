#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#if defined(NDEBUG) && !defined(USE_LIBRARY_AS_STANDALONE)
    #include <app_config.h>
#endif

#include "../application/application.h"
#include "../logger/logger.h"

namespace entix {

int main(int argc, char** argv) {
    Logger::info() << "Creating main application";
    Logger::endline();

    auto usingDefaultConfig = false;

#ifndef NDEBUG
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
#elif !defined(USE_LIBRARY_AS_STANDALONE)
    if (g_app_config_len == 0) {
        Logger::error() << "Application config not found";
        Logger::endline();

        return -1;
    }

    const std::string yaml(g_app_config[0]);
    const auto execPath = std::filesystem::canonical(argv[0]);
    const auto configPath = execPath.parent_path();
#else
    std::filesystem::path configPath; // fallback for compilation error
#endif

    YAML::Node node = YAML::Load(yaml);
    std::string title = "Untitled";
    if (node["Title"]) title = node["Title"].as<std::string>();

    VectorI wSize(600, 400);
    if (node["Size"]) wSize = node["Size"].as<VectorI>();

    std::vector<core::Application::WindowFlag> windowFlags;
    if (node["Flags"]) {
        std::map<std::string, core::Application::WindowFlag> bind = {
            {"shown", core::Application::WindowFlag::SHOWN},
            {"resizable", core::Application::WindowFlag::RESIZABLE},
            {"minimized", core::Application::WindowFlag::MINIMIZED},
            {"maximized", core::Application::WindowFlag::MAXIMIZED},
            {"borderless", core::Application::WindowFlag::BORDERLESS},
            {"always on top", core::Application::WindowFlag::ALWAYS_ON_TOP},
            {"centered", core::Application::WindowFlag::CENTERED},
            {"fullscreen", core::Application::WindowFlag::FULLSCREEN},
            {"fullscreen desktop",
             core::Application::WindowFlag::FULLSCREEN_DESKTOP}};
        for (auto f : node["Flags"])
            windowFlags.push_back(bind[f.as<std::string>()]);
    }

    core::Application application(title, wSize.x, wSize.y, windowFlags);

    application._configPath =
        (node["AssetsFolder"]
             ? (configPath / node["AssetsFolder"].as<std::string>())
             : (configPath / "assets"))
            .string();

    auto& serializer = application.getSerializer();

    if (node["Position"]) {
        auto n = node["Position"];
        if (n.IsSequence()) {
            auto pos = n.as<VectorI>();
            application.setWindowPosition(pos.x, pos.y);
        } else {
            Logger::warn() << "Position should be a sequence of two numbers";
            Logger::endline();
        }
    }

    if (node["FPS"]) application.setFramerate(node["FPS"].as<int>());

#ifndef NDEBUG
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