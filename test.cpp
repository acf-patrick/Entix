#include <iostream>
#include <application/application.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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

			SDL_RenderCopyEx(renderer, texture, NULL, &dest, 0, NULL, SDL_FLIP_NONE);
		});
	}
};

class Button : public Component::script
{
	EventListner event;
	SDL_Texture* texture;
	std::string state = "idle";
	SDL_Rect rect = { 10, 10, 0, 0 };
	std::map<std::string, SDL_Color> palette = {
		{ "idle"    , {  79,  79,  79, 255 } },
		{ "hover"   , { 120, 120, 120, 255 } },
		{ "pressed" , {  45,  45,  45, 255 } }
	};

public:
	Button()
	{
		TTF_Font* font = TTF_OpenFont("font.ttf", 12);
		SDL_Surface *s = TTF_RenderText_Blended(font, "QUIT", {255, 255, 255, 255});
		texture = SDL_CreateTextureFromSurface(Renderer::get().renderer, s);
		rect.w = s->w + 7*2;
		rect.h = s->w + 5*2;
		SDL_FreeSurface(s);
		TTF_CloseFont(font);

		auto& Event = EventManager::get();
		event.listen(Event.MOUSE_BUTTON_DOWN, [&](Entity& e)
		{
			SDL_Point mouse = { Event.mouse.x, Event.mouse.y };
			if (SDL_PointInRect(&mouse, &rect))
				state = "pressed";
		});
		event.listen(Event.MOUSE_BUTTON_UP, [&](Entity& e)
		{
			if (state == "pressed")
				Application::get().quit();
		});
		event.listen(Event.MOUSE_MOTION, [&](Entity& e)
		{
			SDL_Point mouse = { Event.mouse.x, Event.mouse.y };
			state = SDL_PointInRect(&mouse, &rect)?"hover":"idle";
		});
	}
	~Button()
	{
		SDL_DestroyTexture(texture);
	}

	void Render() override
	{
		auto& renderer = Renderer::get();
		renderer.submit([&](SDL_Renderer* renderer)
		{
			SDL_SetRenderDrawColor(renderer, palette[state].r, palette[state].g, palette[state].b ,255);
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderCopy(renderer, texture, NULL, &rect);
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
		entities.create().attach<Button>();
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
