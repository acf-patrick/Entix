#include "texture.h"

#include <SDL_image.h>

#include "../logger/logger.h"
#include "../renderer/renderer.h"

std::map<std::string, SDL_Texture *> Texture::_loadedTextures;

Texture::Texture(const Path &file) { load(file); }

Texture::~Texture() {
    if (_texture) {
        _loadedTextures.erase(_file);
        SDL_DestroyTexture(_texture);
    }
}

void Texture::unload() {
    for (auto &[_, texture] : _loadedTextures) SDL_DestroyTexture(texture);
    _loadedTextures.clear();
}

bool Texture::load(const Path &filePath) {
    if (!filePath.exists()) {
        Logger::error("Texture") << filePath << " does not exist";
        Logger::endline();

        return false;
    }

    std::string file(filePath);

    if (_loadedTextures[file]) {
        _file = file;
        _texture = _loadedTextures[file];

        Logger::info("Texture") << file << " : texture loaded from cache";
        Logger::endline();
    } else {
        _texture =
            IMG_LoadTexture(RenderManager::Get()->renderer, file.c_str());

        if (_texture) {
            _file = file;
            _loadedTextures[file] = _texture;
        } else {
            Logger::error("Texture") << "Failed to load '" << file << "'";
            Logger::endline();

            return false;
        }
    }

    return true;
}

std::string Texture::getName() const { return _file; }

SDL_Texture *Texture::get() const { return _texture; }

VectorI Texture::getSize() const {
    VectorI ret;
    SDL_QueryTexture(_texture, NULL, NULL, &ret.x, &ret.y);
    return ret;
}

void Texture::set(SDL_Texture *texture) {
    _file = "";
    _texture = texture;
}

Texture::operator bool() const { return _texture != NULL; }

void Texture::draw(const VectorI &dst) {
    draw(dst, {false, false}, {1.0f, 1.0f});
}

void Texture::draw(const VectorI &dst, const Vector<bool> &flip,
                   const VectorF &scale) {
    auto s = getSize();
    draw({0, 0, s.x, s.y}, dst, flip, scale);
}

void Texture::draw(const VectorI &dst, const VectorI &center, float rotation,
                   const Vector<bool> &flip, const VectorF &scale) {
    auto s = getSize();
    draw({0, 0, s.x, s.y}, dst, center, rotation, flip, scale);
}

void Texture::draw(const SDL_Rect &src, const VectorI &dst,
                   const Vector<bool> &flip, const VectorF &scale) {
    draw(src, dst, {0, 0}, 0.0f, flip, scale);
}

void Texture::draw(const SDL_Rect &src, const VectorI &dst,
                   const VectorI &center, float rotation,
                   const Vector<bool> &flip, const VectorF &scale) {
    SDL_Rect d = {dst.x, dst.y, int(src.w * scale.x), int(src.h * scale.y)};
    SDL_Point c = {center.x, center.y};
    SDL_RenderCopyEx(RenderManager::Get()->renderer, _texture, &src, &d,
                     rotation, &c, SDL_RendererFlip((flip.y << 1) | flip.x));
}