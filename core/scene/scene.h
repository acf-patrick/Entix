#pragma once

#include "ecs/entity/container.h"

class Scene
{
public:

    Scene() = default;
    virtual ~Scene() = default;

private:

    EntityContainer _container;
};