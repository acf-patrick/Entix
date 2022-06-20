#include "../components.h"

namespace Component {

    std::set<camera*, camera::_compare> camera::instances;

    camera::camera()
    {
        instances.emplace(this);
    }

    camera::~camera()
    {
        instances.erase(this);
        SDL_DestroyTexture(_colorTexture);
        _colorTexture = nullptr;
    }

    void camera::_attachTransform()
    {
        if (!entity->has<Component::transform>())
            entity->attach<Component::transform>();
    }
}
