#pragma once

#include <functional>
#include <SDL2/SDL.h>
#include <string>
#include <queue>
#include <deque>

class Applicaiton;

// Renderer System
class Renderer
{
public:

using Drawer = std::function<void(SDL_Renderer*)>;

    static Renderer& get();
    static void clean();

    // clear a portion of the screen with the given color, default is black.
    void clear(const SDL_Color& color = { 0, 0, 0, 255 });
    void clear(const SDL_Rect&, const SDL_Color& color = { 0, 0, 0, 255 });

    // perform drawing
    void draw();

    // insert drawing function
    void submit(const Drawer&);
    void submit(const Drawer&, std::size_t);

    void update();

    SDL_Renderer* renderer;

private:

    Renderer();
    ~Renderer();

    void empty();

    // There is always one layer remaining
    std::deque<std::queue<Drawer>> layers;

    static Renderer* instance;

};