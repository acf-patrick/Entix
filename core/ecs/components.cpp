#include "components.h"
#include <algorithm>

using Camera = Component::camera;
std::vector<Camera*> Camera::instances;

bool compare(Camera* c1, Camera* c2)
{ return c1->depth < c2->depth; }

Camera::camera()
{
    instances.push_back(this);
    std::sort(instances.begin(), instances.end(), compare);
}

Camera::~camera()
{
    instances.erase(std::remove(instances.begin(), instances.end(), this));
    SDL_DestroyTexture(backgroundImage);
    SDL_DestroyTexture(_colorTexture);
    _colorTexture = backgroundImage = nullptr;
}
