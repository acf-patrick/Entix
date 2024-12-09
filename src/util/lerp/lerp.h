#pragma once

#include <SDL.h>

#include "../geometry/vector.h"

namespace entix {
namespace util {

template <typename T>
T lerp(const T& start, const T& end, float t) {
    return static_cast<T>((1 - t) * start + t * end);
}

template <>
SDL_Color lerp(const SDL_Color& start, const SDL_Color& end, float t) {
    return ((SDL_Color){.r = lerp(start.r, end.r, t),
                        .g = lerp(start.g, end.g, t),
                        .b = lerp(start.b, end.b, t),
                        .a = lerp(start.a, end.a, t)});
}

template <typename T>
Vector<T> lerp(const Vector<T>& start, const Vector<T>& end, float t) {
    return Vector<T>(lerp(start.x, end.x, t), lerp(start.y, end.y, t));
}

}  // namespace util
}  // namespace entix