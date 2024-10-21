/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Definition of Texture
 */

#pragma once

#include <SDL.h>
#include <optional>
#include <unordered_map>

#include "../path/path.h"
#include "../util/geometry/vector.h"

namespace entix {

namespace core {
class Application;
}

/**
 * Texture
 *
 * Prevent memory leak and ensure texture reusability
 * by avoid multiple loading of the same texture
 */
class Texture {
   private:
    std::string _tag = "";

    struct TextureAndCounter {
        SDL_Texture* texture;
        size_t refCounter = 1;

        TextureAndCounter(SDL_Texture* texture) : texture(texture) {}
    };

    static std::unordered_map<std::string, TextureAndCounter> _loadedTextures;

    // Make sure to unload left textures
    static void Clean();

   public:
    Texture() = default;
    Texture(const Path&);

    ~Texture();

    bool load(const Path&);

    // Get filename used to load the texture
    std::string getName() const;

    // Get SDL_Texture* raw data
    SDL_Texture* get() const;

    // Return texture size
    std::optional<VectorI> getSize() const;

    // check if inner SDL Texture is null
    operator bool() const;

    void draw(const VectorI& dst);
    void draw(const VectorI& dst, const Vector<bool>& flip,
              const VectorF& scale);
    void draw(const SDL_Rect& src, const VectorI& dst, const Vector<bool>& flip,
              const VectorF& scale);
    void draw(const VectorI& dst, const VectorI& center, float rotation,
              const Vector<bool>& flip = {false, false},
              const VectorF& scale = {1.0f, 1.0f});
    void draw(const SDL_Rect& src, const VectorI& dst, const VectorI& center,
              float rotation, const Vector<bool>& flip = {false, false},
              const VectorF& scale = {1.0f, 1.0f});

    friend class entix::core::Application;
};

}  // namespace entix
