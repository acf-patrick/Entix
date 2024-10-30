#include <SDL2_gfxPrimitives.h>

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
                         Entity* camera) {
    if (!camera->get<Camera>().contains(rect)) return;

    const auto& cameraPosition = camera->get<Transform>().position;
    VectorI center(int(rect.x + 0.5 * rect.w - cameraPosition.x),
                   int(rect.y + 0.5 * rect.h - cameraPosition.y));

    ellipseColor(renderer, center.x, center.y, int(rect.w * 0.5),
                 int(rect.h * 0.5), 0xffffffff);
}

void Drawer::drawPoint(const tson::Vector2i& position, SDL_Renderer* renderer,
                       Entity* camera) {}

void Drawer::drawRectangle(const SDL_Rect& rect, SDL_Renderer* renderer,
                           Entity* camera) {
    if (!camera->get<Camera>().contains(rect)) return;

    const auto& cameraPosition = camera->get<Transform>().position;
    auto x = rect.x - cameraPosition.x;
    auto y = rect.y - cameraPosition.y;

    rectangleColor(renderer, x + rect.w, y, x, y + rect.h, 0xffffffff);
}

void Drawer::drawPolyline(const tson::Vector2i& position,
                         const std::vector<tson::Vector2i>& vertexes,
                         SDL_Renderer* renderer, Entity* camera) {
    VectorI min(std::numeric_limits<int>::max(),
                std::numeric_limits<int>::max());
    VectorI max(std::numeric_limits<int>::min(),
                std::numeric_limits<int>::min());

    const auto& cameraPosition = camera->get<Transform>().position;

    auto n = vertexes.size();
    auto xs = new Sint16[n];
    auto ys = new Sint16[n];

    for (size_t i = 0; i < n; ++i) {
        auto x = position.x + vertexes[i].x;
        auto y = position.y + vertexes[i].y;

        xs[i] = x - cameraPosition.x;
        ys[i] = y - cameraPosition.y;

        if (x < min.x) min.x = x;
        if (y < min.y) min.y = y;

        if (x > max.x) max.x = x;
        if (y > max.y) max.y = y;
    }

    SDL_Rect boundingBox = {min.x, min.y, max.x - min.x, max.y - min.y};
    if (camera->get<Camera>().contains(boundingBox))
        polygonColor(renderer, xs, ys, n, 0xffffffff);

    delete[] xs;
    delete[] ys;
}

void Drawer::drawText(const tson::Text& text, const tson::Vector2i& position,
                      SDL_Renderer* renderer, Entity* camera) {}

}  // namespace ecs
}  // namespace entix
