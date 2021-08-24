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
    if (file.empty())
        return;
    _texture = IMG_LoadTexture(Renderer->renderer, file.c_str());
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

void Texture::draw(const VectorI& dst)
{
    draw(dst, { false, false }, { 1.0f, 1.0f });
}

void Texture::draw(const VectorI& dst, const Vector<bool>& flip, const VectorF& scale)
{
    auto s = getSize();
    draw({ 0, 0, s.x, s.y }, dst, flip, scale);
}

void Texture::draw(const VectorI& dst, const VectorI& center, float rotation, const Vector<bool>& flip, const VectorF& scale)
{
    auto s = getSize();
    draw({ 0, 0, s.x, s.y }, dst, center, rotation, flip, scale);
}

void Texture::draw(const SDL_Rect& src, const VectorI& dst, const Vector<bool>& flip, const VectorF& scale)
{
    draw(src, dst, {0, 0}, 0.0f, flip, scale);
}

void Texture::draw(const SDL_Rect& src, const VectorI& dst, const VectorI& center, float rotation, const Vector<bool> &flip, const VectorF& scale)
{
    SDL_Rect d = {
        dst.x, dst.y,
        int(src.w*scale.x), int(src.h*scale.y)
    };
    SDL_Point c = { center.x, center.y };
    SDL_RenderCopyEx(Renderer->renderer, _texture, &src, &d, rotation, &c, SDL_RendererFlip((flip.y<<1)|flip.x));
}