#include "serializer.h"
#include "../ecs/components.h"

#include <fstream>
#include <cstdlib>

void Serializer::serializeEntity(Entity& entity)
{
    out << YAML::BeginMap;
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
    }

    if (entity.has<Component::camera>())
    {
        out << YAML::Key << "CameraComponent" << YAML::Value;
        auto& c = entity.get<Component::camera>();
        out << YAML::BeginMap;
        out << YAML::Key << "Size" << YAML::Value << c.size;
        out << YAML::Key << "Destination" << YAML::Value << c.destination;
        out << YAML::Key << "BackgroundColor" << YAML::Value << c.background;
        out << YAML::Key << "BackgroundImage" << YAML::Value << c.backgroundImage.getName();
        out << YAML::Key << "Clear Mode" << YAML::Value << c.clear;
        out << YAML::Comment("0 : NONE, 1 : TEXTURE, 2 : SOLID_COLOR, 3 : TEXTURE_AND_SOLID_COLOR");
        out << YAML::Key << "Flip" << YAML::Value << c.flip;
        out << YAML::Key << "Depth" << YAML::Value << c.depth;
        out << YAML::Key << "Layers" << YAML::Value << c.layers;
        out << YAML::EndMap;
    }
    
    out << YAML::EndMap;
}

void Serializer::serializeScene(Scene* scene)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Name" << YAML::Value << scene->tag;

    out << YAML::Key << "Entities" << YAML::Value;

    out << YAML::BeginSeq;
    scene->entities.for_each([&](Entity& entity)
    {
        serializeEntity(entity);
    });
    out << YAML::EndSeq;

    out << YAML::EndMap;

    system("mkdir -p scenes");
    std::ofstream file("scenes/" + scene->tag + ".yml");
    file << out.c_str();
}

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
