/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * The rendering system
 */

#pragma once

#include <SDL.h>

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "../ecs/components.h"
#include "../manager/manager.h"

namespace entix {
namespace core {

class Application;

// Rendering System
class RenderManager : Manager<RenderManager> {
   public:
    using Camera = ecs::component::Camera;
    using ProcessWithCamera = std::function<void(SDL_Renderer*, ecs::Entity*)>;
    using ProcessWithoutCamera = std::function<void(SDL_Renderer*)>;

    struct Drawer {
        struct Process {
            std::optional<ProcessWithCamera> withCamera;
            std::optional<ProcessWithoutCamera> withoutCamera;
        };

        std::shared_ptr<RenderManager> renderManager = RenderManager::Get();
        std::vector<Process> processes;
        SDL_Texture* target = nullptr;

        SDL_Renderer* renderer = renderManager->renderer;
        int currentTarget = 0;

        Drawer() {
            auto s = renderManager->getSize();
            target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET, s.x, s.y);
            assert(target && "Unable to create texture target for layer\n");
            SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
        }

        ~Drawer() {
            SDL_SetRenderTarget(renderer, NULL);
            SDL_DestroyTexture(target);
        }

        void add(const ProcessWithoutCamera& operation) {
            Process process;
            process.withoutCamera = operation;
            processes.push_back(process);
        }

        void add(const ProcessWithCamera& operation) {
            Process process;
            process.withCamera = operation;
            processes.push_back(process);
        }

        void clear() { processes.clear(); }

        void prepare() {
            SDL_SetRenderTarget(renderer, target);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
        }

        void operator()(ecs::Entity* camera) {
            for (auto& process : processes) {
                if (process.withCamera)
                    process.withCamera->operator()(renderer, camera);
                else
                    process.withoutCamera->operator()(renderer);
            }
        }
    };

    static std::shared_ptr<RenderManager> Get();

    // clear a portion of the screen with the given color, default is black.
    void clear(const SDL_Color& color = {0, 0, 0, 255});

    // clear a portion of the screen with the given color, default is black.
    void clear(const SDL_Rect&, const SDL_Color& color = {0, 0, 0, 255});

    // perform drawing
    void render();

    // submit on all layers
    void submit(const ProcessWithCamera&);

    // submit on all layers
    void submit(const ProcessWithoutCamera&);

    void submit(const ProcessWithCamera&, const std::vector<size_t>& layers);

    void submit(const ProcessWithoutCamera&, const std::vector<size_t>& layers);

    VectorI getSize() const;

    VectorI globalCoordinates(float, float) const;

    VectorI globalCoordinates(const VectorF&) const;

    VectorF viewportCoordinates(int, int) const;

    VectorF viewportCoordinates(const VectorI&) const;

    SDL_Renderer* renderer;

   private:
    // SDL_Texture* view;

    // There is always one layer remaining
    // [layer_id, drawer]
    std::map<int, Drawer> drawers;

    RenderManager();
    ~RenderManager();

    void verifyLayers();

    friend class Application;
    friend class Manager<RenderManager>;
};

}  // namespace core
}  // namespace entix
