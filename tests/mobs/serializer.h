#pragma once

#include "scripts.h"
#include "serializer/serializer.h"

class CustomSerializer : public entix::core::Serializer {
   public:
    void deserializeEntity(YAML::Node& node,
                           entix::ecs::Entity& entity) override {
        Serializer::deserializeEntity(node, entity);

        auto c = node["Mob"];
        if (c) entity.attach<Mob>();

        c = node["Ground"];
        if (c) entity.attach<Ground>();

        c = node["FollowMouseBehavior"];
        if (c) entity.attach<FollowMouseBehavior>();

        c = node["FpsText"];
        if (c) entity.attach<FpsText>();

        c = node["SpriteAnimator"];
        if (c) {
            auto frameDuration = c["FrameDuration"].as<Uint32>();
            entity.attach<SpriteAnimator>(frameDuration);
        }
    }
};