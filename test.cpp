#include <iostream>
#include <application/application.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ecs/ecs.h>

class DrawTexture : public Component::script
{
	int var;
public:

	bool draw = true;

	void onAttach() override
	{
		entity->attach<SDL_Texture*>(IMG_LoadTexture(Renderer::get().renderer, "texture.jpg"));
	}

	void onDetach() override
	{
		SDL_DestroyTexture(entity->get<SDL_Texture*>());
	}

	void Render() override
	{
		auto& renderer = Renderer::get();
		renderer.clear();

		if (draw) renderer.submit([&](SDL_Renderer* renderer) 
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

class Button : public Component::script
{
	EventListner event;

public:
	SDL_Color boxColor = { 255, 255, 0, 255 };

	Button()
	{
		auto& Event = EventManager::get();
		event.listen(Event.MOUSE_BUTTON_UP, [&](Entity& event){ Disable(); });
	}

	void Render() override
	{
		auto& r = Renderer::get();
		r.submit([&](SDL_Renderer* renderer)
		{
			SDL_Rect field = { 0, 0, 64, 32 };
			SDL_SetRenderDrawColor(renderer, boxColor.r, boxColor.g, boxColor.b, 255);
			SDL_RenderFillRect(renderer, &field);
		});
	}

};

class Main : public Scene
{
	bool active = true;

public:

	Main() : Scene("main scene")
	{
		auto& Event = EventManager::get();
		event.listen(Event.QUIT, [&](Entity& entity) { active = false; });
		entities.create().attach<DrawTexture>();
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
