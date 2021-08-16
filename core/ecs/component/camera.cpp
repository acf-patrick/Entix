#include "../components.h"
#include <algorithm>

namespace Component {

    std::vector<camera*> camera::instances;

    bool compare(camera* c1, camera* c2)
    { return c1->depth < c2->depth; }

    camera::camera()
    {
        instances.push_back(this);
        std::sort(instances.begin(), instances.end(), compare);
    }

    camera::~camera()
    {
        instances.erase(std::remove(instances.begin(), instances.end(), this));
        SDL_DestroyTexture(backgroundImage);
        SDL_DestroyTexture(_colorTexture);
        _colorTexture = backgroundImage = nullptr;
    }

    void camera::_attachTransform()
    {
        if (!entity->has<Component::transform>())
            entity->attach<Component::transform>();
    }
}
