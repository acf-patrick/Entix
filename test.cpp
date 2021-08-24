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

class FollowBehavior : public Component::script
{
public:
	int entier;
	char caractere;
	std::string str;
	std::vector<int> tab;
	std::map<int, int> dict;

	FollowBehavior()
	{
		auto& r = get<Component::group>();
		r.content->for_each([](Entity& entity)
		{

		});
	}

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
	void serializeEntity(YAML::Emitter& out, Entity& entity) override
	{
		Serializer::serializeEntity(out, entity);
		
		if (entity.has<FollowBehavior>())
		{
			out << YAML::Key << "FollowBehavior" << YAML::Value;
			out << YAML::BeginMap;
			auto& f = entity.get<FollowBehavior>();
			out << YAML::Key << "Entier" << YAML::Value << f.entier;
			out << YAML::Key << "Str" << YAML::Value << f.str;
			out << YAML::Key << "Tab" << YAML::Value << f.tab;
			out << YAML::Key << "Dict" << YAML::Value << f.dict;
			out << YAML::EndMap;
		}
	}

	void deserializeEntity(YAML::Node& node, Entity& entity) override
	{
		Serializer::deserializeEntity(node, entity);
		auto n = node["FollowBehavior"];
		if (n)
		{
			auto& f = entity.attach<FollowBehavior>();
			f.entier = n["Entier"].as<int>();
			f.str = n["Str"].as<std::string>();
			f.tab = n["Tab"].as<std::vector<int>>();
			f.dict = n["Dict"].as<std::map<int, int>>();
		}
	}
};
