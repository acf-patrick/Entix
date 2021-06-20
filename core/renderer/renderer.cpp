#include "renderer.h"
#include <cassert>

Renderer* Renderer::instance = nullptr;
Renderer& Renderer::get()
{ 
    if (!instance)
        instance = new Renderer;
    return *instance; 
}
void Renderer::clean()
{
    delete instance;
    instance = nullptr;
}

Renderer::Renderer()
{
    empty();
}
Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
}

void Renderer::submit(const Drawer& drawer, std::size_t layer_n)
{
    if (layer_n >= layers.size())
        submit(drawer);
    else
        layers[layer_n].push(drawer);
}

void Renderer::submit(const Drawer& drawer)
{
    layers.back().push(drawer);
}

void Renderer::clear(const SDL_Rect& rect, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Renderer::clear(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void Renderer::draw()
{
    for (auto& layer : layers)
        while (!layer.empty())
        {
            layer.front()(renderer);
            layer.pop();
        }
    SDL_RenderPresent(renderer);

    empty();
}

void Renderer::empty()
{
    layers.clear();
    layers.emplace_back();
}