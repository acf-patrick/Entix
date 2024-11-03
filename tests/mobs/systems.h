#pragma once

#include "ecs/system/system.h"

class WorldSystem : public entix::ecs::ISystem {
    EventListner eventListner;

   public:
    WorldSystem() : ISystem("WorldSystem") {}

    ~WorldSystem() { delete World; }

    bool run() override {
        World->Step(timeStep, 6, 2);
        return true;
    }
};