#include "blur.h"

#include <SDL_image.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "../../logger/logger.h"

Uint32 getPixel(int x, int y, SDL_Surface* surface) {
    if (!surface) return 0;
    if (x < 0 or y < 0 or x >= surface->w or y >= surface->h) return 0;

    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32*)p;
        default:;
    }

    return 0;
}

void setPixel(int x, int y, Uint32 pixel, SDL_Surface* surface) {
    if (!surface) return;
    if (x < 0 or y < 0 or x >= surface->w or y >= surface->h) return;

    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16*)p = pixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32*)p = pixel;
            break;
        default:;
    }
}

namespace entix {
namespace util {

SDL_Surface* boxBlur(const std::string& file_name, int extent) {
    return boxBlur(IMG_Load(file_name.c_str()), extent);
}

SDL_Surface* gaussianBlur(const std::string& file_name, int extent) {
    return gaussianBlur(IMG_Load(file_name.c_str()), extent);
}

SDL_Surface* boxBlur(SDL_Surface* surface, int extent) {
    if (extent <= 0 or !surface) return surface;

    SDL_LockSurface(surface);

    const int radius = extent * 2 + 1;
    int w = surface->w, h = surface->h;
    SDL_Rect sr = {0, 0, w, h};
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, RMASK,
                                            GMASK, BMASK, AMASK);
    if (!ret) {
        Logger::error("Util", "BoxBlur") << "Can't create destination surface!";
        Logger::endline();

        return NULL;
    }

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            int r, g, b, a;
            r = g = b = a = 0;
            for (int x = -extent; x <= extent; ++x)
                for (int y = -extent; y <= extent; ++y) {
                    SDL_Point p = {x + i, y + j};
                    if (SDL_PointInRect(&p, &sr)) {
                        Uint32 px = getPixel(p.x, p.y, surface);
                        Uint8 r0, g0, b0, a0;
                        SDL_GetRGBA(px, surface->format, &r0, &g0, &b0, &a0);
                        r += r0 / (radius * radius);
                        g += g0 / (radius * radius);
                        b += b0 / (radius * radius);
                        a += a0 / (radius * radius);
                    }
                }
            setPixel(i, j, SDL_MapRGBA(ret->format, r, g, b, a), ret);
        }
    }

    SDL_UnlockSurface(surface);
    return ret;
}

SDL_Surface* gaussianBlur(SDL_Surface* surface, int extent) {
    if (extent <= 0 or !surface) return surface;

    SDL_LockSurface(surface);

    const int radius = extent * 2 + 1;
    int w = surface->w, h = surface->h;
    SDL_Rect sr = {0, 0, w, h};
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, RMASK,
                                            GMASK, BMASK, AMASK);
    if (!ret) {
        Logger::error("Util", "GaussianBlur")
            << "Can't create destination surface!";
        Logger::endline();

        return NULL;
    }
    int i, j;

    float sigma = std::max(extent / 2.0, 1.0);
    float var = sigma * sigma;
    float sum = 0;

    // double ker[radius][radius];
    std::vector<std::vector<double>> ker(radius, std::vector<double>(radius));

    for (i = -extent; i <= extent; ++i)
        for (j = -extent; j <= extent; ++j) {
            double k =
                std::exp(-(i * i + j * j) / (2 * var)) / (2 * M_PI * var);
            ker[extent + i][extent + j] = k;
            sum += k;
        }
    for (i = 0; i < radius; ++i)
        for (j = 0; j < radius; ++j) ker[i][j] /= sum;

    for (i = 0; i < w; ++i) {
        for (j = 0; j < h; ++j) {
            double r, g, b, a;
            r = g = b = a = 0;
            for (int x = -extent; x <= extent; ++x)
                for (int y = -extent; y <= extent; ++y) {
                    SDL_Point p = {x + i, y + j};
                    if (SDL_PointInRect(&p, &sr)) {
                        Uint32 px = getPixel(p.x, p.y, surface);
                        Uint8 r0, g0, b0, a0;
                        SDL_GetRGBA(px, surface->format, &r0, &g0, &b0, &a0);
                        double k = ker[x + extent][y + extent];
                        r += r0 * k;
                        g += g0 * k;
                        b += b0 * k;
                        a += a0 * k;
                    }
                }
            setPixel(i, j, SDL_MapRGBA(ret->format, r, g, b, a), ret);
        }
    }

    SDL_UnlockSurface(surface);
    return ret;
}

}  // namespace util
}  // namespace entix