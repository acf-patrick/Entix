#include <core.h>
#include <box2d/box2d.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using Script = Component::script;

const float MtoPX = 80.0f;
const float timeStep = 1 / 60.0f;
b2World* World = new b2World({ 0.0f, 10.0f });

class Mob : public Script
{
public:
	b2Body* body;
	Uint32 lastTick;

	Mob()
	{
		// Dynamic body
		b2BodyDef def;
		def.type = b2_dynamicBody;
		def.position.Set(400 / MtoPX, 100 / MtoPX);
		def.angle = b2_pi / 6;
		body = World->CreateBody(&def);

		b2FixtureDef fdef;
		fdef.density = 1.0f;
		fdef.friction = 0.5f;
		fdef.restitution = 0.2f;
		b2PolygonShape shape;
		shape.SetAsBox(16 / MtoPX, 16 / MtoPX);
		fdef.shape = &shape;
		body->CreateFixture(&fdef);
	}

	void Update() override
	{
		auto pos = body->GetPosition();
		auto& t = get<Component::transform>();
		t.position.x = pos.x * MtoPX;
		t.position.y = pos.y * MtoPX;
		t.rotation = body->GetAngle() * 180 / b2_pi;
	}
};

class FollowMouseBehavior : public Script
{
public:
	FollowMouseBehavior()
	{
		event
			.listen(Input.MOUSE_MOTION, [&](Entity& entity)
				{
					auto& position = get<Component::transform>().position;
					auto event = entity.get<SDL_MouseMotionEvent>();
					position.set(event.x, event.y);
				});
	}
};

class Controller : public Script
{
	FPSmanager fpsManager;

public:
	Controller()
	{
		SDL_initFramerate(&fpsManager);
		SDL_setFramerate(&fpsManager, 60);

		// create ground
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(400 / MtoPX, 400 / MtoPX);
		groundBodyDef.type = b2_staticBody;
		auto ground = World->CreateBody(&groundBodyDef);

		b2PolygonShape groundShape;
		groundShape.SetAsBox(300 / MtoPX, 10 / MtoPX);
		ground->CreateFixture(&groundShape, 0.0f);
	}

	~Controller()
	{
		delete World;
	}

	void Render() override
	{
		Renderer->submit(
			[&](SDL_Renderer* renderer)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				SDL_Rect rect = { 100, 390, 600, 20 };
				SDL_RenderDrawRect(renderer, &rect);
			});
	}

	void Update() override
	{
		SDL_framerateDelay(&fpsManager);
		World->Step(timeStep, 6, 2);
	}

	void onAttach() override
	{
		event
			.listen(Input.QUIT, [](Entity& _) { Application::Get().quit(); })
			.listen(
				Input.MOUSE_BUTTON_UP, [&](Entity& _) {
					auto& e = get<Component::group>().content->create();
					e.useTemplate("assets/prefabs/mob.entt");
					std::cout << "id : " << e.idAsString() << std::endl;

					auto mousePos = Input.mouse.getPosition();
					if (e.has<Mob>())
					{
						auto& body = *e.get<Mob>().body;
						body.SetTransform({ mousePos.x / MtoPX, mousePos.y / MtoPX }, 0.0f);
						std::cout << "entity position set" << std::endl;
					}
				});
	}
};

class FpsText : public Script {
	TTF_Font* font;

	SDL_Rect destination;
	SDL_Texture* texture = nullptr;
	
	Uint32 latestTick;
	std::vector<float> rates;

public:
	FpsText() {
		destination.x = destination.y = 10;
		latestTick = SDL_GetTicks();

		std::string fontPath("assets/fonts/Ubuntu-Regular.ttf");
		font = TTF_OpenFont(fontPath.c_str(), 24);

		if (!font) {
			std::cerr << fontPath << std::endl;
		}
	}

	~FpsText() {
		TTF_CloseFont(font);
		SDL_DestroyTexture(texture);
	}

	void Render() override {
		Renderer->submit([&](SDL_Renderer* renderer) {
			auto currentTick = SDL_GetTicks();
			rates.push_back(1000.0 / (currentTick - latestTick));
			latestTick = currentTick;

			if (rates.size() == 10) {
				float fps = 0.0;
				for (auto rate : rates) {
					fps += rate;
				}
				fps /= 10;
				
				auto fpsText = std::to_string(int(fps));
				fpsText += " FPS";

				auto surface = TTF_RenderText_Solid(font, fpsText.c_str(), { 255, 255, 255 });
				if (texture) {
					SDL_DestroyTexture(texture);
				}

				texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_FreeSurface(surface);

				destination.w = surface->w;
				destination.h = surface->h;

				rates.clear();
			}
			
			if (texture) {
				SDL_RenderCopy(renderer, texture, NULL, &destination);
			}
		});
	}
};

class MySerializer : public Serializer
{
public:
	void deserializeEntity(YAML::Node& node, Entity& entity) override
	{
		Serializer::deserializeEntity(node, entity);
		
		auto c = node["Mob"];
		if (c)
			entity.attach<Mob>();
		
		c = node["Controller"];
		if (c)
			entity.attach<Controller>();
		
		c = node["FollowMouseBehavior"];
		if (c)
			entity.attach<FollowMouseBehavior>();

		c = node["FpsText"];
		if (c)
			entity.attach<FpsText>();
	}
};

Serializer* Application::serializer = new MySerializer;