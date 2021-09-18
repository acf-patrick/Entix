#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

#include "../application/application.h"

int main(int argc, char** argv)
{
    std::cout << "Creating main application" << std::endl;

    std::ostringstream ss;
    {
        std::ifstream cfg((argc > 1)?argv[1]:"app.cfg");
        if (cfg)
            ss << cfg.rdbuf();
        else
            std::cout << "Default configuration used" << std::endl;
    }
    YAML::Node node = YAML::Load(ss.str());
    std::string title = "Untitled";
    if (node["Title"])
        title = node["Title"].as<std::string>();
    VectorI wSize(600, 400);
    if (node["WindowSize"])
        wSize = node["WindowSize"].as<VectorI>();

    int flag(0);
    if (node["Flags"])
    {
        std::map<std::string, SDL_WindowFlags> bind = {
            { "shown", SDL_WINDOW_SHOWN }, 
            { "resizable", SDL_WINDOW_RESIZABLE },
            { "minimized", SDL_WINDOW_MINIMIZED },
            { "maximized", SDL_WINDOW_MAXIMIZED },
            { "borderless", SDL_WINDOW_BORDERLESS },
            { "always on top", SDL_WINDOW_ALWAYS_ON_TOP },
            { "fullscreen", SDL_WINDOW_FULLSCREEN_DESKTOP }
        };
        for (auto f : node["Flags"])
            flag |= bind[f.as<std::string>()];
    }

    auto APP = new Application(title, wSize.x, wSize.y, SDL_WindowFlags(flag));
    assert(APP->serializer && "No serializer declared. Create serializer in global scope!");
    auto& s = *APP->serializer;

    if (node["Scenes"])
        for (auto scene : node["Scenes"])
            s.deserialize("scenes/" + scene.as<std::string>() + ".scn");
    
    APP->run();

    std::cout << "Exiting application" << std::endl;
    delete APP;
    return 0;
}