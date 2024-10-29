#include "../../components.h"

namespace entix {
namespace ecs {

using Drawer = component::Tilemap::Drawer;

std::map<EntityID, Drawer*> Drawer::instances;

void Drawer::onAttach() {
    if (entity) instances[entity->id()] = this;
}

void Drawer::onDistach() {
    if (entity) instances.erase(entity->id());
}

Drawer::~Drawer() { onDistach(); }

void Drawer::drawImage(const std::string& imgPath,
                       const tson::Vector2f& position, SDL_Renderer* renderer,
                       Entity* camera) {}

void Drawer::drawObject(tson::Object object, SDL_Renderer* renderer,
                        Entity* camera) {}

void Drawer::drawEllipse(const SDL_Rect& rect, SDL_Renderer* renderer,
                         Entity* camera) {}

void Drawer::drawPoint(const tson::Vector2i& position, SDL_Renderer* renderer,
                       Entity* camera) {}

void Drawer::drawRectangle(const SDL_Rect& rect, SDL_Renderer* renderer,
                           Entity* camera) {}

void Drawer::drawPolygon(const std::vector<tson::Vector2i>& vertexes,
                         SDL_Renderer* renderer, Entity* camera) {}

void Drawer::drawPolyline(const std::vector<tson::Vector2i>& vertexes,
                          SDL_Renderer* renderer, Entity* camera) {}

void Drawer::drawText(const tson::Text& text, const tson::Vector2i& position,
                      SDL_Renderer* renderer, Entity* camera) {}

}  // namespace ecs
}  // namespace entix
