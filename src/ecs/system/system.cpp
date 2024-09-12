#include "system.h"

#include "../entity/entity.h"
#include "../filter/filter.h"
#include "../../scene/scene.h"

ISystem::ISystem(const std::string& name, IFilter* filter)
    : _filter(filter), _name(name) {}

ISystem::~ISystem() { delete _filter; }

bool ISystem::performOnEntities(Group& entities) {
    _entities = entities.view(*_filter);
    return run();
}

void SystemManager::run() {
    auto& entities = SceneManager::Get()->getActive().getEntities();

    for (auto& [name, system] : _systems) {
        if (!system->performOnEntities(entities)) {
            Logger::error("System") << name << " failed";
            Logger::endline();
        }
    }
}

// static
std::shared_ptr<SystemManager> SystemManager::Get() { return createInstance(); }