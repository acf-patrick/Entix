#include "scene.h"

#include "../application/application.h"
#include "../ecs/system/system.h"

Scene::Scene(const std::string& _tag) : tag(_tag) {
    SceneManager::Get()->push(this);
}

bool Scene::update() {
    if (!_systemsActivated) {
        auto systemManager = SystemManager::Get();

        for (auto& system : _freeSystems) systemManager->useFreeSystem(system);

        for (auto& sequence : _systemGroups)
            systemManager->useSystemSequence(sequence);

        _systemsActivated = true;
    }

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