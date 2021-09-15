#include "scene.h"
#include "../application/application.h"

Scene::Scene(const std::string& _tag) : tag(_tag)
{
    SceneManager->push(this);
}

bool Scene::update()
{
    entities.for_each([](Entity& entity) 
    { 
        entity.Update(); 
    });
    return active;
}

void Scene::render()
{
    entities.for_each([](Entity& entity) 
    { 
        entity.Render(); 
    });
}

void Scene::save(const std::string& fileName)
{
    Application::serializer->serialize(this, fileName);
}

void Scene::setActive()
{
    SceneManager->setActive(tag);
}