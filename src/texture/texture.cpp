#include "texture.h"

#include <SDL_image.h>

#include "../logger/logger.h"
#include "../renderer/renderer.h"

namespace entix {

std::unordered_map<std::string, Texture::TextureAndCounter>
    Texture::_loadedTextures;

Texture::Texture(const Path &file) { load(file); }

Texture::~Texture() {
    if (!_tag.empty()) {
        auto &counter = _loadedTextures[_tag].refCounter;
        if (counter == 1) {
            SDL_DestroyTexture(_loadedTextures[_tag].texture);
            _loadedTextures.erase(_tag);
        } else {
            counter--;
        }
    }
}

void Texture::Clean() {
    for (auto &[_, texture] : _loadedTextures)
        SDL_DestroyTexture(texture.texture);
    _loadedTextures.clear();
}

bool Texture::load(const Path &filePath) {
    if (!filePath.exists()) {
        Logger::error("Texture") << filePath << " does not exist";
        Logger::endline();

        return false;
    }

    std::string file(filePath);

    if (_loadedTextures.find(file) != _loadedTextures.end()) {
        _tag = file;
        _loadedTextures[_tag].refCounter++;

        Logger::info("Texture") << file << " : texture loaded from cache";
        Logger::endline();
    } else {
        auto texture =
            IMG_LoadTexture(core::RenderManager::Get()->renderer, file.c_str());

        if (texture) {
            _tag = file;
            _loadedTextures[file] = TextureAndCounter(texture);
        } else {
            Logger::error("Texture") << "Failed to load '" << file << "'";
            Logger::endline();

            return false;
        }
    }

    return true;
}

std::string Texture::getName() const { return _tag; }

SDL_Texture *Texture::get() const {
    return _tag.empty() ? nullptr : _loadedTextures[_tag].texture;
}

std::optional<VectorI> Texture::getSize() const {
    if (_tag.empty()) return std::nullopt;

    VectorI ret;
    SDL_QueryTexture(_loadedTextures[_tag].texture, NULL, NULL, &ret.x, &ret.y);
    return ret;
}

Texture::operator bool() const {
    return _tag.empty() ? false : (_loadedTextures[_tag].texture != nullptr);
}

void Texture::draw(const VectorI &dst) {
    draw(dst, {false, false}, {1.0f, 1.0f});
}

void Texture::draw(const VectorI &dst, const Vector<bool> &flip,
                   const VectorF &scale) {
    if (_tag.empty()) return;

    auto s = getSize();
    draw({0, 0, s->x, s->y}, dst, flip, scale);
}

void Texture::draw(const VectorI &dst, const VectorI &center, float rotation,
                   const Vector<bool> &flip, const VectorF &scale) {
    if (_tag.empty()) return;

    auto s = getSize();
    draw({0, 0, s->x, s->y}, dst, center, rotation, flip, scale);
}

void Texture::draw(const SDL_Rect &src, const VectorI &dst,
                   const Vector<bool> &flip, const VectorF &scale) {
    draw(src, dst, {0, 0}, 0.0f, flip, scale);
}

void Texture::draw(const SDL_Rect &src, const VectorI &dst,
                   const VectorI &center, float rotation,
                   const Vector<bool> &flip, const VectorF &scale) {
    if (_tag.empty()) return;

    SDL_Rect d = {dst.x, dst.y, int(src.w * scale.x), int(src.h * scale.y)};
    SDL_Point c = {center.x, center.y};
    SDL_RenderCopyEx(core::RenderManager::Get()->renderer,
                     _loadedTextures[_tag].texture, &src, &d, rotation, &c,
                     SDL_RendererFlip((flip.y << 1) | flip.x));
}

}  // namespace entix