#include "serializer.h"
#include "../ecs/components.h"

#include <fstream>
#include <sstream>
#include <cstdlib>

template <typename VType>
YAML::Emitter& operator<< (YAML::Emitter& out, const Vector<VType>& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<< (YAML::Emitter& out, const SDL_Rect& rect)
{
    out << YAML::Flow;
    out << YAML::BeginSeq;
    out << YAML::Flow << YAML::BeginSeq << rect.x << rect.y << YAML::EndSeq;
    out << YAML::Flow << YAML::BeginSeq << rect.w << rect.h << YAML::EndSeq;
    out << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<< (YAML::Emitter& out, const SDL_Color& color)
{
    out << YAML::Flow;
    out << YAML::BeginSeq;
    out << (int)color.r << (int)color.g << (int)color.b << (int)color.a;
    out << YAML::EndSeq;
    return out;
}

Scene* Serializer::deserialize(const std::string& source)
{
    auto error = []() -> Scene*
    {
        std::cerr << "Invalid file format!" << std::endl;
        return nullptr;
    };
    std::ifstream file(source);
    if (!file)
    {
        std::cerr << "Scene file : " << source << " doesn't exist!" << std::endl;
        return nullptr;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    YAML::Node node = YAML::Load(ss.str());

    auto name = node["Name"];
    if (!name)
        return error();
    Scene* scene = new Scene(name.as<std::string>());

    auto entities = node["Entities"];
    if (!entities)
        return error();
    for (auto entity : entities)
        deserializeEntity(entity, scene->entities.create(entity["ID"].as<EntityID>()));

    if (scene)
        std::cout << source << " loaded." << std::endl;
    else
        std::cerr << "Failed to load " << source << std::endl;

    return scene;
}

void Serializer::serialize(Scene* scene)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Name" << YAML::Value << scene->tag;

    out << YAML::Key << "Entities" << YAML::Value;

    out << YAML::BeginSeq;
    scene->entities.for_each([&](Entity& entity)
    {
        out << YAML::BeginMap;
        serializeEntity(out, entity);
        out << YAML::EndMap;
    });
    out << YAML::EndSeq;

    out << YAML::EndMap;

    std::string output = "scenes/" + scene->tag + ".scn";
    system("mkdir -p scenes");
    std::ofstream file(output);
    file << out.c_str();

    std::cout << "Scene serialized to " << output << std::endl;
}

void Serializer::deserializeEntity(YAML::Node& node, Entity& entity)
{
    YAML::Node n;
    n = node["TagComponent"];
    if (n)
        entity.attach<Component::tag>().content = n["Tag"].as<std::string>();

    n = node["TransformComponent"];
    if (n)
    {
        auto& t = entity.attach<Component::transform>();
        if (n["Position"])
            t.position = n["Position"].as<VectorD>();
        if (n["Scale"])
            t.scale = n["Scale"].as<VectorF>();
        if (n["Rotation"])
            t.rotation = n["Rotation"].as<double>();
    }

    n = node["SpriteComponent"];
    if (n)
    {
        auto& s = entity.attach<Component::sprite>();
        if (n["Texture"])
            s.texture.load(n["Texture"].as<std::string>());
        if (n["Centered"])
            s.centered = n["Centered"].as<bool>();
        if (n["Offset"])
            s.offset = n["Offset"].as<VectorI>();
        if (n["Flip"])
            s.flip = n["Flip"].as<Vector<bool>>();
        if (n["FramesNumber"])
            s.framesNumber = n["FramesNumber"].as<VectorI>();
        if (n["Frame"])
            s.frame = n["Frame"].as<int>();
        if (n["RegionEnabled"])
            s.regionEnabled = n["RegionEnabled"].as<bool>();
        if (n["Region"])s.region = n["Region"].as<SDL_Rect>();
    }

    n = node["SpriteRendererComponent"];
    if (n)
        entity.attach<Component::spriteRenderer>();

    n = node["CameraComponent"];
    if (n)
    {
        auto& c = entity.attach<Component::camera>();
        if (n["Size"])
            c.size = n["Size"].as<VectorF>();
        if (n["Destination"])
            c.destination = n["Destination"].as<VectorF>();
        if (n["BackgroundColor"])
            c.background = n["BackgroundColor"].as<SDL_Color>();
        if (n["BackgroundImage"])
            c.backgroundImage.load(n["BackgroundImage"].as<std::string>());
        if (n["ClearMode"])
            c.clear = Component::camera::ClearMode(n["ClearMode"].as<int>());
        if (n["Flip"])
            c.flip = n["Flip"].as<Vector<bool>>();
        if (n["Depth"])
            c.depth = n["Depth"].as<int>();
        if (n["Layers"])
            c.layers = n["Layers"].as<std::vector<int>>();
    }
}

void Serializer::serializeEntity(YAML::Emitter& out, Entity& entity)
{
    out << YAML::Key << "ID" << YAML::Value << entity.id();

    if (entity.has<Component::tag>())
    {
        out << YAML::Key << "TagComponent" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "Tag" << YAML::Value << entity.get<Component::tag>().content;
        out << YAML::EndMap;
    }

    if (entity.has<Component::transform>())
    {
        out << YAML::Key << "TransformComponent" << YAML::Value;
        auto& t = entity.get<Component::transform>();
        out << YAML::BeginMap;
        out << YAML::Key << "Position" << YAML::Value << t.position;
        out << YAML::Key << "Scale" << YAML::Value << t.scale;
        out << YAML::Key << "Rotation" << YAML::Value << t.rotation;
        out << YAML::EndMap;
    }

    if (entity.has<Component::sprite>())
    {
        out << YAML::Key << "SpriteComponent" << YAML::Value;
        auto& s = entity.get<Component::sprite>();
        out << YAML::BeginMap;
        out << YAML::Key << "Texture" << YAML::Value << s.texture.getName();
        out << YAML::Key << "Centered" << YAML::Value << s.centered;
        out << YAML::Key << "Offset" << YAML::Value << s.offset;
        out << YAML::Key << "Flip" << YAML::Value << s.flip;
        out << YAML::Key << "FramesNumber" << YAML::Value << s.framesNumber;
        out << YAML::Key << "Frame" << YAML::Value << s.frame;
        out << YAML::Key << "RegionEnabled" << YAML::Value << s.regionEnabled;
        out << YAML::Key << "Region" << YAML::Value << s.region;
        out << YAML::EndMap;
    }

    // using map in case SpriteRenderer has propreties in the futur
    if (entity.has<Component::spriteRenderer>())
        out << YAML::Key << "SpriteRendererComponent" << YAML::Comment("Native Script derived class");

    if (entity.has<Component::camera>())
    {
        out << YAML::Key << "CameraComponent" << YAML::Value;
        auto& c = entity.get<Component::camera>();
        out << YAML::BeginMap;
        out << YAML::Key << "Size" << YAML::Value << c.size;
        out << YAML::Key << "Destination" << YAML::Value << c.destination;
        out << YAML::Key << "BackgroundColor" << YAML::Value << c.background;
        out << YAML::Key << "BackgroundImage" << YAML::Value << c.backgroundImage.getName();
        out << YAML::Key << "ClearMode" << YAML::Value << c.clear;
        out << YAML::Key << "Flip" << YAML::Value << c.flip;
        out << YAML::Key << "Depth" << YAML::Value << c.depth;
        out << YAML::Key << "Layers" << YAML::Value << c.layers;
        out << YAML::EndMap;
    }
}
