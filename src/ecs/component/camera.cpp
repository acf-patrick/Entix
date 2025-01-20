#include <limits>

#include "../../renderer/renderer.h"
#include "../components.h"

namespace entix {
namespace ecs {
namespace component {

std::multiset<Camera*, Camera::_compare> Camera::instances;

Camera::Camera() { instances.emplace(this); }

Camera::~Camera() {
    for (auto it = instances.begin(); it != instances.end();) {
        if (*it == this) {
            it = instances.erase(it);
        } else {
            ++it;
        }
    }

    SDL_DestroyTexture(_colorTexture);
    _colorTexture = nullptr;
}

void Camera::_attachTransform() {
    if (!entity->has<component::Transform>())
        entity->attach<component::Transform>();
}

// static
SDL_Rect Camera::MergeBoundingBoxes() {
    SDL_Rect boundingBox = {
        std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
        std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

    for (auto camera : instances) {
        if (!camera->entity) continue;

        const auto& cameraPosition = camera->entity->get<Transform>().position;
        auto rendererSize = core::RenderManager::Get()->getSize();
        VectorI cameraSize(rendererSize.x * camera->size.x,
                           rendererSize.y * camera->size.y);

        boundingBox.x = std::min(boundingBox.x, (int)cameraPosition.x);
        boundingBox.y = std::min(boundingBox.y, (int)cameraPosition.y);
        boundingBox.w =
            std::max(boundingBox.w, int(cameraPosition.x + cameraSize.x));
        boundingBox.h =
            std::max(boundingBox.h, int(cameraPosition.y + cameraSize.y));
    }

    boundingBox.w -= boundingBox.x;
    boundingBox.h -= boundingBox.y;

    return boundingBox;
}

bool Camera::contains(const SDL_Rect& rect) {
    if (!entity) return false;

    auto rendererManager = core::RenderManager::Get();
    if (!rendererManager) return false;

    auto rendererSize = rendererManager->getSize();

    auto cameraRect = getBoundingBox();

    SDL_Rect wholeScreen = {0, 0, rendererSize.x, rendererSize.y};
    SDL_IntersectRect(&cameraRect, &wholeScreen, &cameraRect);

    SDL_Rect visible;
    SDL_IntersectRect(&cameraRect, &rect, &visible);

    return visible.w > 0 && visible.h > 0;
}

SDL_Rect Camera::getBoundingBox() {
    const auto& position = entity->get<Transform>().position;
    auto rendererSize = core::RenderManager::Get()->getSize();

    SDL_Rect rect = {(int)position.x, (int)position.y,
                     int(rendererSize.x * size.x),
                     int(rendererSize.y * size.y)};
    return rect;
}

}  // namespace component
}  // namespace ecs
}  // namespace entix
