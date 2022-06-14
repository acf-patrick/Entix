#pragma once

#include <vector>
#include <yaml-cpp/yaml.h>

#include "../ecs/entity/entity.h"
#include "../util/geometry/vector.h"
#include "../scene/scene.h"

// operator << override
template <typename T>
YAML::Emitter& operator<< (YAML::Emitter&, const Vector<T>&);
YAML::Emitter& operator<< (YAML::Emitter&, const SDL_Rect&);
YAML::Emitter& operator<< (YAML::Emitter&, const SDL_Color&);

class Application;

// Serializer class
// override serialize/desrialize-Entity for more functionality
// Application::serializer to use unique instance of this class
class Serializer
{
    static int _cnt;
public:
    Serializer();

    virtual void serialize(Scene*, const std::string& fileName = "");
    virtual Scene* deserialize(const std::string&);

    // Serialize entity
    virtual void serializeEntity(YAML::Emitter&, Entity&);
    
    // Deserialize entity
    virtual void deserializeEntity(YAML::Node&, Entity&);

protected:
    virtual ~Serializer() = default;

friend class Application;
};

// encode/decode specialization
namespace YAML {
    
template <typename T>
struct convert<Vector<T>>
{
    static Node encode(const Vector<T>& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        return node;
    }

    static bool decode(const Node& node, Vector<T>& rhs)
    {
        if (!node.IsSequence() || node.size() != 2)
            return false;
        rhs.x = node[0].as<T>();
        rhs.y = node[1].as<T>();
        return true;
    }
};

template <>
struct convert<SDL_Rect>
{
    static Node encode(const SDL_Rect& rhs)
    {
        Node node = Load("[[], []]");
        node[0].push_back(rhs.x); node[0].push_back(rhs.y);
        node[0].push_back(rhs.w); node[0].push_back(rhs.h);
        return node;
    }

    static bool decode(const Node& node, SDL_Rect& rhs)
    {
        auto check = [](const Node& node) -> bool
        {
            return node.IsSequence() and node.size() == 2;
        };
        if (!check(node))
            return false;
        if (!(check(node[0]) and check(node[1])))
            return false;
        
        rhs = {
            node[0][0].as<int>(), node[0][1].as<int>(),
            node[1][0].as<int>(), node[1][1].as<int>()
        };

        return true;
    }
};

template <>
struct convert<SDL_Color>
{
    static Node encode(const SDL_Color& rhs)
    {
        Node node;
        node.push_back(rhs.r);
        node.push_back(rhs.g);
        node.push_back(rhs.b);
        node.push_back(rhs.a);
        return node;
    }

    static bool decode(const Node& node, SDL_Color& rhs)
    {
        if (!node.IsSequence() or node.size() != 4)
            return false;
        
        rhs.r = node[0].as<Uint8>();
        rhs.g = node[1].as<Uint8>();
        rhs.b = node[2].as<Uint8>();
        rhs.a = node[3].as<Uint8>();

        return true;
    }
};

}