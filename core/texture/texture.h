#pragma once

#include <SDL2/SDL.h>

#include "../util/vector.h"

// Equivalent class for SDL_Texture
class Texture
{
private:
    std::string _file = "";
    SDL_Texture* _texture = nullptr;

    void _free();

public:
    Texture() = default;
    Texture(const std::string&);

    ~Texture();

    void load(const std::string&);

    std::string getName() const;
    SDL_Texture* getTexture() const;

    VectorI getSize() const;

    void setTexture(SDL_Texture*);

    void draw(const SDL_Rect&, const SDL_Rect&, float rotation = 0.0f);
    void draw(const SDL_Rect&, const SDL_Rect&, const VectorI&, float, SDL_RendererFlip flip = SDL_FLIP_NONE);
};