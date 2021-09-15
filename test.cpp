#include <core.h>
#include <box2d/box2d.h>
#include <SDL2/SDL_image.h>

using Script = Component::script;

const float MtoPX = 80.0f;
const float timeStep = 1/60.0f;
b2World* World = new b2World({0.0f, 10.0f});

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
		def.position.Set(400/MtoPX, 100/MtoPX);
		def.angle = b2_pi/6;
		body = World->CreateBody(&def);

		b2FixtureDef fdef;
		fdef.density = 1.0f;
		fdef.friction = 0.5f;
		fdef.restitution = 0.4f;
		b2PolygonShape shape;
		shape.SetAsBox(16/MtoPX, 16/MtoPX);
		fdef.shape = &shape;
		body->CreateFixture(&fdef);
	}

 	void Update() override
	{
		auto pos = body->GetPosition();
		auto& t = get<Component::transform>();
		t.position.x = pos.x*MtoPX;
		t.position.y = pos.y*MtoPX;
		t.rotation = body->GetAngle()*180/b2_pi;
	}
 };

class Controller : public Script
{
public:
	Controller()
	{
		// create ground
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(400/MtoPX, 400/MtoPX);
		groundBodyDef.type = b2_staticBody;
		auto ground = World->CreateBody(&groundBodyDef);
		
		b2PolygonShape groundShape;
		groundShape.SetAsBox(300/MtoPX, 10/MtoPX);
		ground->CreateFixture(&groundShape, 0.0f);
	}
	
	~Controller()
	{
		delete World;
	}

	void Render() override
	{
		Renderer->submit([&](SDL_Renderer* renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_Rect rect = { 100, 390, 600, 20 };
			SDL_RenderDrawRect(renderer, &rect);
		});
	}

	void Update() override
	{
		World->Step(timeStep, 6, 2);
	}

	void onAttach() override
	{
		event.listen(Input.QUIT, [](Entity& entity)
		{
			APP->quit();
		});
		event.listen(Input.MOUSE_BUTTON_UP, [&](Entity& entity)
		{
			auto& e = get<Component::group>().content->create();
			e.useTemplate("prefabs/mob.entt");
			auto mousePos = Input.mouse.getPosition();
 			if (e.has<Mob>())
			{
				auto& body = *e.get<Mob>().body;
				body.SetTransform({mousePos.x/MtoPX, mousePos.y/MtoPX}, 0.0f);
				std::cout << "entity position set" << std::endl;
			}
			else
			{
				auto& pos = e.get<Component::transform>().position;
				pos = mousePos;
			}
 			std::cout << "created" << std::endl;
   		});
	}
};

class FollowMouseBehavior : public Script
{
	bool follow = false;
public:
	FollowMouseBehavior()
	{
		event.listen(Input.MOUSE_BUTTON_UP, [&](Entity& entity)
		{
			follow = !follow;
		});
	}

	void Update() override
	{
		if (follow)
		{
			auto& cameraPos = get<Component::camera>().destination;
			auto rs = Renderer->getSize();
			auto mouse = Input.mouse.getPosition();
			cameraPos.x = mouse.x/float(rs.x);
			cameraPos.y = mouse.y/float(rs.y);
		}
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
	}
};
Serializer* Application::serializer = new MySerializer;