#pragma once

#include <functional>
#include <SDL2/SDL.h>
#include <algorithm>
#include <string>
#include <queue>
#include <map>

#include <ecs/components.h>

// Renderer System
class Renderer
{
public:
using Camera = Component::camera;
using Process = std::function<void(SDL_Renderer*)>;

    struct Drawer
    {
        std::queue<Process> process;
        SDL_Texture* target = nullptr;

        Drawer()
        {
            auto& r = Renderer::get();
            auto  s = r.getSize();
            target = SDL_CreateTexture(r.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, s.x, s.y);
            assert(target && "Unable to create texture target for layer\n");
            SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
        }
        ~Drawer()
        {
            SDL_DestroyTexture(target);
        }
        void add(const Process& p)
        {
            process.push(p);
        }
        void clear()
        {
            std::queue<Process> empty;
            std::swap(empty, process);
        }
        void prepare()
        {
            SDL_SetRenderTarget(Renderer::get().renderer, target);
        }
        void operator()()
        {
            if (process.empty())
                return;
            process.front()(Renderer::get().renderer);
            process.pop();
        }
    };

    static Renderer& get();
    static void clean();

    // clear a portion of the screen with the given color, default is black.
    void clear(const SDL_Color& color = { 0, 0, 0, 255 });
    void clear(const SDL_Rect&, const SDL_Color& color = { 0, 0, 0, 255 });

    // perform drawing
    void draw();

    // use n-th layer to perform drawing
    // default : first layer (index 0)
    void submit(const Process&, std::size_t index = 0);

    Vector<int> getSize() const;

    Vector<int> globalCoordinates(float, float) const;

    Vector<int> globalCoordinates(const Vector<float>&) const; 

    Vector<float> viewportCoordinates(int, int) const;

    Vector<float> viewportCoordinates(const Vector<int>&) const;

    SDL_Renderer* renderer;

private:

    Renderer();
    ~Renderer();

    // SDL_Texture* view;

    // There is always one layer remaining
    std::map<int, Drawer> layers;

    std::vector<Camera*> cameras;

    static Renderer* instance;

};