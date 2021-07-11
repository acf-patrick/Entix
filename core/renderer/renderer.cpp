#include "renderer.h"
#include <application/application.h>
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
{}
Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
}

void Renderer::submit(const Process& drawer, std::size_t layer_n)
{
    layers[layer_n].add(drawer);
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
    for (auto& [_, layer] : layers)
    {
        layer.prepare();
        layer();
    }
    SDL_SetRenderTarget(renderer, NULL);

    for (auto c : Camera::instances)
    {
        auto t = c->entity->get<Component::transform>();
        auto position   = t.position;
        auto scale      = t.scale;
        auto rotation   = t.rotation;
        auto viewport   = c->destination;
        auto size       = c->size;
        auto flip       = SDL_RendererFlip((c->flip.y<<1)|c->flip.x);

        auto rs = getSize();
        int w = rs.x, h = rs.y;

        SDL_Rect rect = {
            int(position.x), int(position.y),
            int(size.x*w), int(size.y*h)
        };
        SDL_FRect dest = {
            viewport.x*w, viewport.y*h,
            scale.x*rect.w, scale.y*rect.h
        };

        auto all = c->clear == c->TEXTURE_AND_SOLID_COLOR;
        if (all)
            c->clear = c->SOLID_COLOR;

        auto draw = [&](SDL_Texture* target)
        {
            if (c->clear == c->SOLID_COLOR)
            {
                if (!c->_colorTexture)
                {
                    auto& t = c->_colorTexture;
                    t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
                    SDL_SetRenderTarget(renderer, t);
                    clear(c->background);
                    SDL_SetRenderTarget(renderer, NULL);
                }
                SDL_RenderCopyExF(renderer, c->_colorTexture, &rect, &dest, rotation, NULL, flip);
                
                if (all)
                    c->clear = c->TEXTURE;
            }
            if (c->clear == c->TEXTURE)
            {
                SDL_RenderCopyExF(renderer, c->backgroundImage, &rect, &dest, rotation, NULL, flip);
                
                if (all)
                    c->clear = c->TEXTURE_AND_SOLID_COLOR;
            }

            SDL_RenderCopyExF(renderer, target, &rect, &dest, rotation, NULL, flip);
        };

        for (auto index : c->layers)
            draw(layers[index].target);
    }


    // camera draws
    // SDL_RenderCopy(renderer, view, NULL, NULL);
    SDL_RenderPresent(renderer);

}

VectorI Renderer::globalCoordinates(float x, float y) const
{
    auto size = getSize();
    return VectorI(int(size.x*x), int(size.y*y));
}

VectorI Renderer::globalCoordinates(const VectorF& v) const
{ return globalCoordinates(v.x, v.y); }

VectorF Renderer::viewportCoordinates(int x, int y) const
{
    auto size = getSize();
    return VectorI(x/float(size.x), y/float(size.y));
}

VectorF Renderer::viewportCoordinates(const VectorI& v) const
{ return viewportCoordinates(v.x, v.y); }

VectorI Renderer::getSize() const
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return VectorI(w, h);
}