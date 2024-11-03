#pragma once

#include <SDL.h>
#include <yaml-cpp/yaml.h>

#include "../util/geometry/vector.h"

// operator << override
template <typename T>
YAML::Emitter& operator<<(YAML::Emitter& out, const entix::Vector<T>& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter&, const SDL_Rect&);
YAML::Emitter& operator<<(YAML::Emitter&, const SDL_Color&);

// encode/decode specialization
namespace YAML {

template <typename T>
struct convert<entix::Vector<T>> {
    static Node encode(const entix::Vector<T>& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        return node;
    }

    static bool decode(const Node& node, entix::Vector<T>& rhs) {
        if (!node.IsSequence() || node.size() != 2) return false;
        rhs.x = node[0].as<T>();
        rhs.y = node[1].as<T>();
        return true;
    }
};

template <>
struct convert<SDL_Rect> {
    static Node encode(const SDL_Rect& rhs) {
        Node node = Load("[[], []]");
        node[0].push_back(rhs.x);
        node[0].push_back(rhs.y);
        node[0].push_back(rhs.w);
        node[0].push_back(rhs.h);
        return node;
    }

    static bool decode(const Node& node, SDL_Rect& rhs) {
        auto check = [](const Node& node) -> bool {
            return node.IsSequence() and node.size() == 2;
        };
        if (!check(node)) return false;
        if (!(check(node[0]) and check(node[1]))) return false;

        rhs = {node[0][0].as<int>(), node[0][1].as<int>(), node[1][0].as<int>(),
               node[1][1].as<int>()};

        return true;
    }
};

template <>
struct convert<SDL_Color> {
    static Node encode(const SDL_Color& rhs) {
        Node node;
        node.push_back(rhs.r);
        node.push_back(rhs.g);
        node.push_back(rhs.b);
        node.push_back(rhs.a);
        return node;
    }

    static bool decode(const Node& node, SDL_Color& rhs) {
        if (!node.IsSequence() or node.size() != 4) return false;

        rhs.r = node[0].as<Uint8>();
        rhs.g = node[1].as<Uint8>();
        rhs.b = node[2].as<Uint8>();
        rhs.a = node[3].as<Uint8>();

        return true;
    }
};

}  // namespace YAML