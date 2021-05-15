/* #pragma once

#include <SDL2/SDL.h>
#include <string>

class Applicaiton;

// Renderer System
class Renderer
{
public:

    static Renderer& get();
    static void clean();

private:


    SDL_Renderer* renderer;
    SDL_Rect viewport;

    static Renderer* instance;

friend class Application;
}; */