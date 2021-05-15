#include <iostream>
#include <application/application.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ecs/ecs.h>

// TODO : add default scene so the manager won't never be empty

class Main : public Scene
{
public:

	Main(SDL_Renderer* _renderer) : Scene("main scene"), renderer(_renderer)
	{}
	~Main()
	{
		SDL_DestroyTexture(texture);
	}

private:

	bool update()
	{
		return true;
	}

	void draw()
	{
    	SDL_Point wSize = {800, 600}, tSize;
    	SDL_Rect src, dest;
    	SDL_Texture* texture;

    	texture = IMG_LoadTexture(renderer, "texture.jpg");
		SDL_QueryTexture(texture, NULL, NULL, &tSize.x, &tSize.y);

		dest = {
			int(0.25*(wSize.x)), int(0.25*(wSize.y)),
			int(0.5*wSize.x), int(0.5*wSize.y)
		};

    	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    	SDL_RenderClear(renderer);
    	SDL_RenderCopyEx(renderer, texture, NULL, &dest, 45, NULL, SDL_FLIP_NONE);

    	SDL_RenderPresent(renderer);
	}

	SDL_Renderer* renderer;
	SDL_Texture*  texture;
};

class App : public Application
{
public:
    App() : Application("test", 800, 600)
    { scene.add(new Main(_renderer)); }

};

Application* createApp()
{
    return new App();
}
