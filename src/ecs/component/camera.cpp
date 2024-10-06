#include "../components.h"

namespace entix {
namespace ecs {
namespace component {

std::set<Camera*, Camera::_compare> Camera::instances;

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

}  // namespace component
}  // namespace ecs
}  // namespace entix
