#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <core.h>
/*
class Button : public Component::script
{
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

		event.listen(Input.MOUSE_BUTTON_DOWN, [&](Entity& e)
		{
			SDL_Point mouse = { Input.mouse.x, Input.mouse.y };
			state = "idle";
			if (SDL_PointInRect(&mouse, &rect))
				state = "pressed";
		});
		event.listen(Input.MOUSE_BUTTON_UP, [&](Entity& e)
		{
			SDL_Point mouse = { Input.mouse.x, Input.mouse.y };
			if (SDL_PointInRect(&mouse, &rect))
				Application::get().quit();
		});
		event.listen(Input.MOUSE_MOTION, [&](Entity& e)
		{
			SDL_Point mouse = { Input.mouse.x, Input.mouse.y };
			state = SDL_PointInRect(&mouse, &rect)?"hover":"idle";
		});
	}
	~Button()
	{
		SDL_DestroyTexture(texture);
	}

	void Render() override
	{
		auto& renderer = ;
		renderer.submit([&](SDL_Renderer* renderer)
		{
			SDL_SetRenderDrawColor(renderer, palette[state].r, palette[state].g, palette[state].b ,255);
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderCopy(renderer, texture, NULL, &rect);
		});
	}

};
*/

using Script = Component::script;

class Controller : public Script
{
public:
	Controller()
	{
		event.listen(Input.QUIT, [](Entity& e)
		{
			APP->quit();
		});
	}
	void onDetach() override
	{
		SceneManager->getActive().save("scene");
	}
};

class FollowBehavior : public Script
{
public:
	void Update() override
	{
		const int speed = 1;
		auto& position = get<Component::transform>().position;
		if (Input.keys[SDL_SCANCODE_UP])
			position.y -= speed;
		if (Input.keys[SDL_SCANCODE_DOWN])
			position.y += speed;
		if (Input.keys[SDL_SCANCODE_LEFT])
			position.x -= speed;
		if (Input.keys[SDL_SCANCODE_RIGHT])
			position.x += speed;
	}
/*
	{
		auto& r = Renderer::get();
		auto& mouse = EventManager::get().mouse;
		auto& camera = get<Component::camera>();
		auto& pos = camera.position;
		auto& dest	= camera.destination;
		auto  size 	= r.globalCoordinates(camera.size);

		pos.x = mouse.x - 0.5*size.x;
		pos.y = mouse.y - 0.5*size.y;

		dest.x = pos.x;
		dest.y = pos.y;
		dest = r.viewportCoordinates(dest);
	}
*/
};

class MySerializer : public Serializer
{
public:
	void deserializeEntity(YAML::Node& node, Entity& entity) override
	{
		Serializer::deserializeEntity(node, entity);
		if (node["FollowBehavior"])
			entity.attach<FollowBehavior>();
		if (node["Controller"])
			entity.attach<Controller>();
	}
};

Serializer* Application::serializer = new MySerializer();