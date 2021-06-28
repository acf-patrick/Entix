#pragma once

#include <functional>
#include <SDL2/SDL.h>
#include <string>
#include <queue>
#include <deque>

#include <ecs/components.h>

// Renderer System
class Renderer
{
public:
using Camera = Component::camera;
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

    Vector<int> getSize() const;

    Vector<int> globalCoordinates(float, float) const;

    Vector<int> globalCoordinates(const Vector<float>&) const; 

    Vector<float> viewportCoordinates(float, float) const;

    Vector<float> viewportCoordinates(const Vector<float>&) const;

    void setRenderer(SDL_Renderer*);
    SDL_Renderer* renderer;

private:

    Renderer();
    ~Renderer();

    void empty();

    SDL_Texture* view;

    // There is always one layer remaining
    std::deque<std::queue<Drawer>> layers;

    std::vector<Camera*> cameras;

    static Renderer* instance;

};