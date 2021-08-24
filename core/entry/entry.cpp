#include <iostream>
#include <fstream>
#include <sstream>
#include <yaml/yaml.h>

#include "../application/application.h"
#include "../serializer/serializer.h"

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

    Application app(title, wSize.x, wSize.y);
    Serializer s;
    for (auto scene : scenes)
        SceneManager->push(s.deserialize("scenes/" + scene.as<std::string>() + ".scn"));
    app.run();

    std::cout << "Exiting application" << std::endl;
    return 0;
}