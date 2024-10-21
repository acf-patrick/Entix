#include "../../components.h"

namespace entix {
namespace ecs {

using Drawer = component::Tilemap::Drawer;

std::map<EntityID, Drawer*> Drawer::instances;

Drawer::Drawer() { instances[entity->id()] = this; }

void Drawer::onDistach() { instances.erase(entity->id()); }

Drawer::~Drawer() { onDistach(); }

void Drawer::drawImage(const std::string& imgPath,
                       const tson::Vector2f& position, SDL_Renderer* renderer) {

}

void Drawer::drawObject(tson::Object object, SDL_Renderer* renderer) {}

void Drawer::drawEllipse(const SDL_Rect& rect, SDL_Renderer* renderer) {}

void Drawer::drawPoint(const tson::Vector2i& position, SDL_Renderer* renderer) {

}

void Drawer::drawRectangle(const SDL_Rect& rect, SDL_Renderer* renderer) {}

void Drawer::drawPolygon(const std::vector<tson::Vector2i>& vertexes,
                         SDL_Renderer* renderer) {}

void Drawer::drawPolyline(const std::vector<tson::Vector2i>& vertexes,
                          SDL_Renderer* renderer) {}

void Drawer::drawText(const tson::Text& text, const tson::Vector2i& position,
                      SDL_Renderer* renderer) {}

}  // namespace ecs
}  // namespace entix
