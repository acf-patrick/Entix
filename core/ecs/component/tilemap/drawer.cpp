#include "../../components.h"

using Drawer = Component::Tilemap::Drawer;

std::map<EntityID, Drawer*> Drawer::instances;

void Drawer::onAttach()
{
    instances[entity->id()] = this;
}

void Drawer::onDistach()
{
    instances.erase(entity->id());
}

Drawer::~Drawer()
{
    onDistach();
}

void Drawer::drawImage(const std::string& imgPath, SDL_Renderer* renderer)
{

}