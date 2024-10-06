#pragma once

#include <SDL_ttf.h>
#include <box2d/box2d.h>
#include <core.h>

#include "world.h"

using namespace entix::ecs;
using namespace entix::core;

using Script = entix::ecs::component::Script;

const float MtoPX = 80.0f;
const float timeStep = 1 / 30.0f;

class Mob : public Script {
   public:
    b2Body* body;
    Uint32 lastTick;

    Mob() {
        // Dynamic body
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(400 / MtoPX, 100 / MtoPX);
        def.angle = b2_pi / 6;
        body = World->CreateBody(&def);

        b2FixtureDef fdef;
        fdef.density = 1.0f;
        fdef.friction = 0.5f;
        fdef.restitution = 0.2f;
        b2PolygonShape shape;
        shape.SetAsBox(16 / MtoPX, 16 / MtoPX);
        fdef.shape = &shape;
        body->CreateFixture(&fdef);
    }

    void Update() override {
        auto pos = body->GetPosition();
        auto& t = get<entix::ecs::component::Transform>();
        t.position.x = pos.x * MtoPX;
        t.position.y = pos.y * MtoPX;
        t.rotation = body->GetAngle() * 180 / b2_pi;
    }
};

class FollowMouseBehavior : public Script {
   public:
    FollowMouseBehavior() {
        event.listen(Input::Event::MOUSE_MOTION, [&](Entity& entity) {
            auto& position = get<entix::ecs::component::Transform>().position;
            auto event = entity.get<SDL_MouseMotionEvent>();
            position.set(event.x, event.y);
        });
    }
};

class Ground : public Script {
   public:
    Ground() {
        // create ground
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(400 / MtoPX, 400 / MtoPX);
        groundBodyDef.type = b2_staticBody;
        auto ground = World->CreateBody(&groundBodyDef);

        b2PolygonShape groundShape;
        groundShape.SetAsBox(300 / MtoPX, 10 / MtoPX);
        ground->CreateFixture(&groundShape, 0.0f);
    }

    void Render() override {
        RenderManager::Get()->submit([&](SDL_Renderer* renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_Rect rect = {100, 390, 600, 20};
            SDL_RenderDrawRect(renderer, &rect);
        });
    }
};

class FpsText : public Script {
    TTF_Font* font;

    SDL_Rect destination;
    SDL_Texture* texture = nullptr;

    Uint32 latestTick;
    std::vector<float> rates;

   public:
    FpsText() {
        destination.x = destination.y = 10;
        latestTick = SDL_GetTicks();

        std::string fontPath("assets/fonts/Ubuntu-Regular.ttf");
        font = TTF_OpenFont(fontPath.c_str(), 24);

        if (!font) {
            entix::Logger::error("FpsText") << "Unable to load " << fontPath;
            entix::Logger::endline();
        }
    }

    ~FpsText() {
        TTF_CloseFont(font);
        SDL_DestroyTexture(texture);
    }

    void Render() override {
        RenderManager::Get()->submit([&](SDL_Renderer* renderer) {
            auto currentTick = SDL_GetTicks();
            rates.push_back(1000.0 / (currentTick - latestTick));
            latestTick = currentTick;

            if (rates.size() == 10) {
                float fps = 0.0;
                for (auto rate : rates) {
                    fps += rate;
                }
                fps /= 10;

                auto fpsText = std::to_string(int(fps));
                fpsText += " FPS";

                auto surface = TTF_RenderText_Solid(font, fpsText.c_str(),
                                                    {255, 255, 255});
                if (texture) {
                    SDL_DestroyTexture(texture);
                }

                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);

                destination.w = surface->w;
                destination.h = surface->h;

                rates.clear();
            }

            if (texture) {
                SDL_RenderCopy(renderer, texture, NULL, &destination);
            }
        });
    }
};

class SpriteAnimator : public Script {
    // frame duration in milliseconds
    Uint32 frameDuration;

    Uint32 latestTick;

   public:
    SpriteAnimator(Uint32 frameDuration) : frameDuration(frameDuration) {
        latestTick = SDL_GetTicks();
    }

    void Update() override {
        auto currentTick = SDL_GetTicks();
        if (currentTick - latestTick >= frameDuration) {
            latestTick = currentTick;

            auto& sprite = get<entix::ecs::component::Sprite>();
            sprite.frame++;
            sprite.frame %= sprite.framesNumber.x;

            // skip first frame
            if (sprite.frame == 0) {
                sprite.frame = 1;
            }
        }
    }
};