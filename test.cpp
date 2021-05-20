#include <iostream>
#include <application/application.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ecs/ecs.h>



class Main : public Scene
{
private: 
	bool active = true;

public:

	Main() : Scene("main scene")
	{
		auto& event		= EventManager::get();
    	texture = IMG_LoadTexture(Renderer::get().renderer, "texture.jpg");

		event.connect(event.quit, [&](Entity& entity) { 
			Application::get().quit();
		});
		event.connect(event.keydown, [&](Entity& entity) {
			if (event.keys[SDL_SCANCODE_ESCAPE])
				active = false;
		});
	}
	~Main() override
	{
		SDL_DestroyTexture(texture);
	}

private:

	bool update() override
	{
		Renderer::get().submit([&](SDL_Renderer* renderer)
		{
			SDL_Point wSize = {800, 600}, tSize;
			SDL_Rect src, dest;

			SDL_QueryTexture(texture, NULL, NULL, &tSize.x, &tSize.y);

			dest = {
				int(0.25*(wSize.x)), int(0.25*(wSize.y)),
				int(0.5*wSize.x), int(0.5*wSize.y)
			};

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_RenderCopyEx(renderer, texture, NULL, &dest, 45, NULL, SDL_FLIP_NONE);

			SDL_RenderPresent(renderer);
		});

		return active;
	}

	SDL_Texture*  texture;
};

class App : public Application
{
public:
    App() : Application("test", 800, 600)
    { 
		scene.push(new Main);
	}

};

Application* createApp()
{
    return new App();
}
