#pragma once

#include <ecs/ecs.h>
#include <scene/scene.h>

class MoveCamera : public entix::ecs::Script {
   public:
    void Update() override {
        using Input = entix::core::Input;
        const float speed = 2.0;
        auto& camera = entix::core::SceneManager::Get()
                           ->getActive()
                           .getEntity("main camera")
                           ->get<entix::ecs::component::Transform>()
                           .position;

        if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera.y += speed;

        if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera.y -= speed;

        if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera.x -= speed;

        if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera.x += speed;
    }
}; 