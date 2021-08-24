#pragma once

#include <SDL2/SDL.h>

#include "../util/vector.h"

// Equivalent class for SDL_Texture
class Texture
{
private:
    std::string _file = "";
    SDL_Texture* _texture = nullptr;

public:
    Texture() = default;
    Texture(const std::string&);

    ~Texture();

    void load(const std::string&);

    std::string getName() const;
    SDL_Texture* getTexture() const;

    VectorI getSize() const;

    void setTexture(SDL_Texture*);

    // check if inner SDL Texture is null
    operator bool() const;

    void draw(const VectorI& dst);
    void draw(const VectorI& dst, const Vector<bool>& flip, const VectorF& scale);
    void draw(const SDL_Rect& src, const VectorI& dst, const Vector<bool>& flip, const VectorF& scale);
    void draw(const VectorI& dst, const VectorI& center, float rotation, const Vector<bool>& flip = { false, false }, const VectorF& scale = { 1.0f, 1.0f });
    void draw(const SDL_Rect& src, const VectorI& dst, const VectorI& center, float rotation, const Vector<bool>& flip = { false, false }, const VectorF& scale = { 1.0f, 1.0f });
};