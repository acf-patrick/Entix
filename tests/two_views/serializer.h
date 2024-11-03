#pragma once

#include <serializer/serializer.h>

#include "./components/move_camera.h"

class Serializer : public entix::core::Serializer {
   public:
    virtual void deserializeEntity(YAML::Node& node,
                                   entix::ecs::Entity& entity) override {
        entix::core::Serializer::deserializeEntity(node, entity);
        if (node["MoveFirstCamera"]) entity.attach<MoveFirstCamera>();
        if (node["MoveSecondCamera"]) entity.attach<MoveSecondCamera>();
    }
};