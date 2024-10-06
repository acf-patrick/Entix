#include "scene.h"

#include "../application/application.h"
#include "../ecs/system/system.h"

namespace entix {
namespace core {

const std::string Scene::Event::LOADED = "scene loaded";
const std::string Scene::Event::CHANGED = "scene changed";

Scene::Scene(const std::string& _tag) : tag(_tag) {
    SceneManager::Get()->push(this);
}

bool Scene::update() {
    if (!_systemsActivated) {
        auto systemManager = ecs::SystemManager::Get();

        for (auto& system : _freeSystems) systemManager->useFreeSystem(system);

        for (auto& sequence : _systemGroups)
            systemManager->useSystemSequence(sequence);

        _systemsActivated = true;
    }

    _entities.for_each([](ecs::Entity& entity) { entity.Update(); });
    return active;
}

void Scene::render() {
    _entities.for_each([](ecs::Entity& entity) { entity.Render(); });
}

void Scene::save(const std::string& fileName) {
    Application::Get().getSerializer().serialize(this, fileName);
}

void Scene::setActive() { SceneManager::Get()->setActive(tag); }

ecs::Group& Scene::getEntities() { return _entities; }

ecs::Entity& Scene::createEntity() { return _entities.create(); }

}  // namespace core
}  // namespace entix
