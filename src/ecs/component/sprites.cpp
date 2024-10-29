#include "../../renderer/renderer.h"
#include "../components.h"

namespace entix {
namespace ecs {
namespace component {

void SpriteRenderer::Render() {
    if (!has<Sprite>()) {
        Logger::warn()
            << "SpriteRenderer-error : Entity must have a sprite component "
               "to render!";
        Logger::endline();

        return;
    }

    auto& spriteComponent = get<Sprite>();
    if (!spriteComponent.texture) return;  // no texture to draw

    core::RenderManager::Get()->submit([&](SDL_Renderer* renderer,
                                           Entity* camera) {
        if (!has<Transform>())
            attach<Transform>();  // using default position, rotation and
                                  // scale factor

        const auto& cameraPosition = camera->get<Transform>().position;
        auto& transform = get<Transform>();

        auto pos = transform.position;
        pos -= cameraPosition;

        auto& texture = spriteComponent.texture;
        const auto& scale = transform.scale;
        auto rotation = transform.rotation;
        auto tSize = *texture.getSize();

        SDL_Rect src;
        VectorI frameSize;
        int w, h;

        // check bounds
        if (spriteComponent.framesNumber.x <= 0 or
            spriteComponent.framesNumber.y <= 0 or
            spriteComponent.frame >=
                spriteComponent.framesNumber.x * spriteComponent.framesNumber.y)
            return;

        // select area
        if (spriteComponent.regionEnabled) {
            src.x = spriteComponent.region.x;
            src.y = spriteComponent.region.y;
            w = spriteComponent.region.w;
            h = spriteComponent.region.h;
        } else {  // use whole texture
            src.x = src.y = 0;
            w = tSize.x;
            h = tSize.y;
        }

        // compute frame size
        frameSize.x = w / spriteComponent.framesNumber.x;
        frameSize.y = h / spriteComponent.framesNumber.y;

        // compute source rect
        src.x += (spriteComponent.frame % spriteComponent.framesNumber.x) *
                 frameSize.x;
        src.y += (spriteComponent.frame / spriteComponent.framesNumber.x) *
                 frameSize.y;
        src.w = frameSize.x;
        src.h = frameSize.y;

        // destination
        pos += spriteComponent.offset;

        // center destination
        if (spriteComponent.centered) pos -= {src.w * 0.5, src.h * 0.5};

        SDL_Rect boundingBox = {(int)pos.x, (int)pos.y, int(scale.x * src.w),
                                int(scale.y * src.h)};

        auto& cameraComponent = camera->get<Camera>();

        if (cameraComponent.contains(boundingBox))
            texture.draw(src, {int(pos.x), int(pos.y)}, {src.w / 2, src.h / 2},
                         rotation, spriteComponent.flip, scale);
    });
}

void Sprite::setTexture(const std::string& fileName) { texture.load(fileName); }

void Sprite::setFrame(int x, int y) {
    if (x >= framesNumber.x || y >= framesNumber.y) return;
    frame = x * framesNumber.x + y;
}

}  // namespace component
}  // namespace ecs
}  // namespace entix
