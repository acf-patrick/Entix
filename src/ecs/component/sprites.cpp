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

    core::RenderManager::Get()->submit([&](SDL_Renderer* renderer,
                                           Entity* camera) {
        auto& spriteComponent = get<Sprite>();
        if (!spriteComponent.texture) return;  // no texture to draw

        if (!has<Transform>())
            attach<Transform>();  // using default position, rotation and
                                  // scale factor

        const auto& cameraPosition = camera->get<Transform>().position;
        const auto& transform = get<Transform>();

        auto clampedPosition = transform.position;
        clampedPosition -=
            cameraPosition;  // compute position relative to camera

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
        frameSize.y = h / (float)spriteComponent.framesNumber.y;
        frameSize.x = w / (float)spriteComponent.framesNumber.x;

        // compute source rect
        src.x += (spriteComponent.frame % spriteComponent.framesNumber.x) *
                 frameSize.x;
        src.y += (spriteComponent.frame / spriteComponent.framesNumber.x) *
                 frameSize.y;
        src.w = frameSize.x;
        src.h = frameSize.y;

        // apply offset
        clampedPosition += spriteComponent.offset;

        // center destination
        if (spriteComponent.centered)
            clampedPosition -= {src.w * 0.5, src.h * 0.5};

        SDL_Rect boundingBox = {(int)transform.position.x,
                                (int)transform.position.y, int(scale.x * src.w),
                                int(scale.y * src.h)};

        auto& cameraComponent = camera->get<Camera>();

        if (cameraComponent.contains(boundingBox))
            texture.draw(src, {int(clampedPosition.x), int(clampedPosition.y)},
                         {src.w / 2, src.h / 2}, rotation, spriteComponent.flip,
                         scale);
    });
}

void Sprite::setTexture(const std::string& fileName) { texture.load(fileName); }

std::optional<SDL_Rect> Sprite::getBoundingBox() const {
    Transform mockTransform;
    return getBoundingBox(mockTransform);
}

std::optional<SDL_Rect> Sprite::getBoundingBox(
    const Transform& entityTransform) const {
    SDL_Rect boundingBox = {.x = (int)entityTransform.position.x,
                            .y = (int)entityTransform.position.y,
                            .w = 0,
                            .h = 0};

    VectorI textureSize;
    if (regionEnabled)
        textureSize.set(region.w, region.h);
    else if (auto sizeOrNull = texture.getSize(); sizeOrNull)
        textureSize = *sizeOrNull;
    else
        return std::nullopt;

    // slice tiles
    textureSize.x /= (float)framesNumber.x;
    textureSize.y /= (float)framesNumber.y;

    // apply scale and setup size
    boundingBox.w = textureSize.x * entityTransform.scale.x;
    boundingBox.h = textureSize.y * entityTransform.scale.y;

    if (centered) {
        boundingBox.x -= boundingBox.w * 0.5;
        boundingBox.y -= boundingBox.h * 0.5;
    }

    // apply offset
    boundingBox.x -= offset.x;
    boundingBox.y -= offset.y;

    return boundingBox;
}

void Sprite::setFrame(int x, int y) {
    if (x >= framesNumber.x || y >= framesNumber.y) return;
    frame = x * framesNumber.x + y;
}

}  // namespace component
}  // namespace ecs
}  // namespace entix
