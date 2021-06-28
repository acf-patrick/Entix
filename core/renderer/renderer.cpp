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
{
    empty();
}
Renderer::~Renderer()
{
    SDL_DestroyTexture(view);
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
    SDL_SetRenderTarget(renderer, view);
    for (auto& layer : layers)
        while (!layer.empty())
        {
            layer.front()(renderer);
            layer.pop();
        }
    SDL_SetRenderTarget(renderer, NULL);

    for (auto c : Camera::instancies)
    {
        auto viewport   = c->destination;
        auto size       = c->size;
        auto position   = c->position;
        auto scale      = c->scale;
        auto rotation   = c->rotation;
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

        // draw background
        {
            auto all = c->clear == c->TEXTURE_AND_SOLID_COLOR;
            if (all)
                c->clear = c->SOLID_COLOR;

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
        }

        SDL_RenderCopyExF(renderer, view, &rect, &dest, rotation, NULL, flip);
    }

    // camera draws
    // SDL_RenderCopy(renderer, view, NULL, NULL);
    SDL_RenderPresent(renderer);

    empty();
}

void Renderer::empty()
{
    layers.clear();
    layers.emplace_back();
}

void Renderer::setRenderer(SDL_Renderer* r)
{
    renderer = r;
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    view = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(view, SDL_BlendMode::SDL_BLENDMODE_BLEND);
    assert(view && "unable to create texture target for rendering");
}

Vector<int> Renderer::globalCoordinates(float x, float y) const
{
    auto size = getSize();
    return Vector<int>(int(size.x*x), int(size.y*y));
}

Vector<int> Renderer::globalCoordinates(const Vector<float>& v) const
{ return globalCoordinates(v.x, v.y); }

Vector<float> Renderer::viewportCoordinates(float x, float y) const
{
    auto size = getSize();
    return Vector<float>(x/size.x, y/size.y);
}

Vector<float> Renderer::viewportCoordinates(const Vector<float>& v) const
{ return viewportCoordinates(v.x, v.y); }

Vector<int> Renderer::getSize() const
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return Vector<int>(w, h);
}