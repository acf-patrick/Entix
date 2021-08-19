#include "texture.h"
#include "../renderer/renderer.h"

#include <SDL2/SDL_image.h>

Texture::Texture(const std::string& file)
{
    load(file);
}

Texture::~Texture()
{
    SDL_DestroyTexture(_texture);
}

void Texture::load(const std::string& file)
{
    _texture = IMG_LoadTexture(Renderer::get().renderer, file.c_str());
    if (_texture)
        _file = file;
    else
        std::cerr << "Failed to load " << file << std::endl;
}

std::string Texture::getName() const
{ 
    return _file;    
}

SDL_Texture* Texture::getTexture() const
{
    return _texture;
}

VectorI Texture::getSize() const
{
    VectorI ret;
    SDL_QueryTexture(_texture, NULL, NULL, &ret.x, &ret.y);
    return ret;
}

void Texture::setTexture(SDL_Texture* texture)
{
    _file = "";
    _texture = texture;
}

Texture::operator bool() const
{
    return _texture != NULL;
}

void Texture::draw(const SDL_Rect& src, const SDL_Rect& dst, float rotation)
{
    auto renderer = Renderer::get().renderer;
    SDL_RenderCopyEx(renderer, _texture, &src, &dst, rotation, NULL, SDL_FLIP_NONE);
}

void Texture::draw(const SDL_Rect& src, const SDL_Rect& dst, const VectorI& center, float rotation, SDL_RendererFlip flip)
{
    auto renderer = Renderer::get().renderer;
    SDL_Point c = { center.x, center.y };
    SDL_RenderCopyEx(renderer, _texture, &src, &dst, rotation, &c, flip);
}