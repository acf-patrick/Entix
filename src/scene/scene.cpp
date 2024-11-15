#include "scene.h"

#include "../application/application.h"
#include "../ecs/system/system.h"

namespace entix {
namespace core {

const std::string Scene::Event::LOADED = "scene loaded";
const std::string Scene::Event::CHANGED = "scene changed";
const std::string Scene::FOLDER = "scenes";
const std::string Scene::FILE_EXTENSION = ".scn";

Scene::Scene(const std::string& _tag) : tag(_tag) {}

bool Scene::update() {
    static Uint32 lastTick(SDL_GetTicks());

    if (!_systemsActivated) {
        auto systemManager = ecs::SystemManager::Get();

        for (auto& system : _freeSystems) systemManager->useFreeSystem(system);

        for (auto& sequence : _systemGroups)
            systemManager->useSystemSequence(sequence);

        _systemsActivated = true;
    }

    auto currentTick = SDL_GetTicks();
    const auto dt = currentTick - lastTick;

    _entities.for_each([dt](ecs::Entity& entity) { entity.Update(dt); });
    lastTick = currentTick;

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

ecs::Entity* Scene::getEntity(const std::string& tag) { return _entities[tag]; }

ecs::Entity& Scene::createEntity() { return _entities.create(); }

}  // namespace core
}  // namespace entix
