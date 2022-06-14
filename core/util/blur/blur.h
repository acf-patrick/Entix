#pragma once

#include <string>
#include <SDL2/SDL.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define RMASK 0xff000000
    #define GMASK 0x00ff0000
    #define BMASK 0x0000ff00
    #define AMASK 0x000000ff
#else
    #define AMASK 0xff000000
    #define BMASK 0x00ff0000
    #define GMASK 0x0000ff00
    #define RMASK 0x000000ff
#endif

// blur file image
// using box algorithm
SDL_Surface* boxBlur(const std::string&, int extent = 1);

// blur surface
// using box algorithm
SDL_Surface* boxBlur(SDL_Surface*, int extent = 1);

// blur file image
// using Gauss algorithm
SDL_Surface* gaussianBlur(const std::string&, int extent = 1);

// blur surface
// using Gauss algorithm
SDL_Surface* gaussianBlur(SDL_Surface*, int extent = 1);