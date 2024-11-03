#include "renderer.h"

#include <cassert>

#include "../application/application.h"

namespace entix {
namespace core {

RenderManager::RenderManager() {}
RenderManager::~RenderManager() { SDL_DestroyRenderer(renderer); }

void RenderManager::submit(const ProcessWithCamera& drawer) {
    std::vector<size_t> layers;
    for (auto& [layer, _] : drawers) layers.push_back(layer);

    submit(drawer, layers);
}

void RenderManager::submit(const ProcessWithoutCamera& drawer) {
    std::vector<size_t> layers;
    for (auto& [layer, _] : drawers) layers.push_back(layer);

    submit(drawer, layers);
}

void RenderManager::submit(const ProcessWithCamera& drawer,
                           const std::vector<size_t>& layers) {
    for (auto layer_n : layers) drawers[layer_n].add(drawer);
}

void RenderManager::submit(const ProcessWithoutCamera& drawer,
                           const std::vector<size_t>& layers) {
    for (auto layer_n : layers) drawers[layer_n].add(drawer);
}

void RenderManager::clear(const SDL_Rect& rect, const SDL_Color& color) {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderManager::clear(const SDL_Color& color) {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void RenderManager::verifyLayers() {
    if (drawers.empty()) {
        auto& _ = drawers[0];  // create layer 0
    }
}

void RenderManager::render() {
    verifyLayers();

    for (auto camera : Camera::instances) {
        auto& transform = camera->entity->get<ecs::component::Transform>();
        const auto& scale = transform.scale;
        // const auto& position = transform.position;
        auto rotation = transform.rotation;
        const auto& viewport = camera->destination;
        auto size = camera->size;
        auto flip = SDL_RendererFlip((camera->flip.y << 1) | camera->flip.x);

        auto rs = getSize();
        int w = rs.x, h = rs.y;

        if (size.x > 1) size.x = 1;
        if (size.y > 1) size.y = 1;

        SDL_Rect rect = {0, 0, int(size.x * w), int(size.y * h)};
        SDL_FRect dest = {viewport.x * w, viewport.y * h, scale.x * rect.w,
                          scale.y * rect.h};

        for (auto layerIndex : camera->layers) {
            {  // clearing process
                if ((int)camera->clear & (int)Camera::ClearMode::SOLID_COLOR) {
                    if (rotation == 0.0f) {
                        SDL_Rect dst = {int(dest.x), int(dest.y), int(dest.w),
                                        int(dest.h)};
                        clear(dst, camera->background);
                    } else {
                        if (auto& colorTexture = camera->_colorTexture;
                            !colorTexture) {
                            colorTexture = SDL_CreateTexture(
                                renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, w, h);
                            SDL_SetRenderTarget(renderer, colorTexture);
                            clear(camera->background);
                            SDL_SetRenderTarget(renderer, NULL);
                        }
                        SDL_RenderCopyExF(renderer, camera->_colorTexture, NULL,
                                          &dest, rotation, NULL, SDL_FLIP_NONE);
                    }
                }

                if ((int)camera->clear & (int)Camera::ClearMode::TEXTURE)
                    SDL_RenderCopyExF(renderer, camera->backgroundImage.get(),
                                      &rect, &dest, rotation, NULL, flip);
            }

            auto& drawer = drawers[layerIndex];
            drawer.prepare();
            drawer(camera->entity);

            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopyExF(renderer, drawer.target, &rect, &dest, rotation,
                              NULL, flip);
        };
    }

    for (auto camera : Camera::instances)
        for (auto layerIndex : camera->layers) drawers[layerIndex].clear();

    SDL_RenderPresent(renderer);
}

VectorI RenderManager::globalCoordinates(float x, float y) const {
    auto size = getSize();
    return VectorI(int(size.x * x), int(size.y * y));
}

VectorI RenderManager::globalCoordinates(const VectorF& v) const {
    return globalCoordinates(v.x, v.y);
}

VectorF RenderManager::viewportCoordinates(int x, int y) const {
    auto size = getSize();
    return VectorI(x / float(size.x), y / float(size.y));
}

VectorF RenderManager::viewportCoordinates(const VectorI& v) const {
    return viewportCoordinates(v.x, v.y);
}

VectorI RenderManager::getSize() const {
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return VectorI(w, h);
}

// static
std::shared_ptr<RenderManager> RenderManager::Get() { return createInstance(); }

}  // namespace core
}  // namespace entix
