#include <iostream>
#include <fstream>
#include <sstream>
#include <yaml/yaml.h>

#include "../application/application.h"

Application* APP;

int main(int argc, char** argv)
{
    std::cout << "Creating main application" << std::endl;

    std::ifstream cfg("app.cfg");
    if (!cfg)
    {
        std::cerr << "No configuration file found!" << std::endl;
        exit(0);
    }
    std::ostringstream ss;
    ss << cfg.rdbuf();
    YAML::Node node = YAML::Load(ss.str());
    auto title  = node["Title"].as<std::string>();
    auto wSize  = node["WindowSize"].as<VectorI>();
    auto scenes = node["Scenes"];

    APP = new Application(title, wSize.x, wSize.y);
    assert(APP->serializer && "No serializer declared. Create serializer in global scope!");
    auto& s = *APP->serializer;
    for (auto scene : scenes)
        s.deserialize("scenes/" + scene.as<std::string>() + ".scn");
    
    APP->run();

    std::cout << "Exiting application" << std::endl;
    delete APP;
    return 0;
}