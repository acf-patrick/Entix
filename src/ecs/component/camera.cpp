#include <limits>

#include "../../renderer/renderer.h"
#include "../components.h"

namespace entix {
namespace ecs {
namespace component {

std::set<Camera *, Camera::_compare> Camera::instances;

Camera::Camera() { instances.emplace(this); }

Camera::~Camera() {
    instances.erase(this);
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

        const auto &cameraPosition = camera->entity->get<Transform>().position;
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

}  // namespace component
}  // namespace ecs
}  // namespace entix
