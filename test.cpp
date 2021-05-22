#include <iostream>
#include <application/application.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ecs/ecs.h>

class DrawTexture : public Component::script
{
public:

	bool draw = false;
	DrawTexture()
	{
		auto& Event = EventManager::get();
		Event.connect(Event.keydown, [&](Entity& entity) {
			if (Event.keys[SDL_SCANCODE_SPACE])
				draw = !draw;
		});
	}

	void onAttach() override
	{
		auto& self = *entity;
		self.attach<SDL_Texture*>(IMG_LoadTexture(Renderer::get().renderer, "texture.jpg"));
	}

	void onDestroy() override
	{
		SDL_DestroyTexture(entity->get<SDL_Texture*>());
	}

	void Render() override
	{
		auto& r = Renderer::get();
		r.clear();

		if (draw) r.submit([&](SDL_Renderer* renderer) 
		{
			auto texture = entity->get<SDL_Texture*>();

			SDL_Point wSize = {800, 600}, tSize;
			SDL_Rect src, dest;

			SDL_QueryTexture(texture, NULL, NULL, &tSize.x, &tSize.y);

			dest = {
				int(0.25*(wSize.x)), int(0.25*(wSize.y)),
				int(0.5*wSize.x), int(0.5*wSize.y)
			};

			SDL_RenderCopyEx(renderer, texture, NULL, &dest, 45, NULL, SDL_FLIP_NONE);
		});
	}
};

class Main : public Scene
{
private: 
	bool active = true;

public:

	Main() : Scene("main scene")
	{
		auto& Event = EventManager::get();
		Event.connect(Event.quit, [&](Entity& entity) { 
			active = false;
		});
		entities.create().attachScript<DrawTexture>();
	}

private:

	bool update() override
	{
		Scene::update();
		return active;
	}
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
