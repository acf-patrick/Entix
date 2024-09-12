#include "scene.h"

#include "../application/application.h"

Scene::Scene(const std::string& _tag) : tag(_tag) {
    SceneManager::Get()->push(this);
}

bool Scene::update() {
    _entities.for_each([](Entity& entity) { entity.Update(); });
    return active;
}

void Scene::render() {
    _entities.for_each([](Entity& entity) { entity.Render(); });
}

void Scene::save(const std::string& fileName) {
    Application::Get().getSerializer().serialize(this, fileName);
}

void Scene::setActive() { SceneManager::Get()->setActive(tag); }

Group& Scene::getEntities() { return _entities; }