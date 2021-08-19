#pragma once

#include <yaml-cpp/yaml.h>

#include "../ecs/entity/entity.h"
#include "../util/vector.h"
#include "../scene/scene.h"

class Serializer
{
public:
    YAML::Emitter out;

    Serializer() = default;
    virtual ~Serializer() = default;

    virtual void serializeEntity(Entity&);
    virtual void serializeScene(Scene*);
};

template<typename T>
YAML::Emitter& operator<< (YAML::Emitter&, const Vector<T>&);
YAML::Emitter& operator<< (YAML::Emitter&, const SDL_Rect&);
YAML::Emitter& operator<< (YAML::Emitter&, const SDL_Color&);
