#include "system.h"

#include <algorithm>
#include <mutex>

#if defined(__linux__)
#include <pthread.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

#include "../../event/event.h"
#include "../../scene/scene.h"
#include "../entity/entity.h"
#include "../filter/filter.h"

const std::string SystemManager::Event::SYSTEM_DEACTIVATED = "system deactivated";
const std::string SystemManager::Event::SYSTEM_ACTIVATED = "system activated";

ISystem::ISystem(const std::string& name, bool runOnlyOnce)
    : _name(name), _runOnlyOnce(runOnlyOnce) {}

ISystem::ISystem(const std::string& name, IFilter* filter, bool runOnlyOnce)
    : _filter(filter), _name(name), _runOnlyOnce(runOnlyOnce) {}

ISystem::~ISystem() { delete _filter; }

bool ISystem::performOnEntities(Group& entities) {
    _entities = _filter ? entities.view(*_filter) : entities.getEntities();
    return run();
}

void ISystem::activate() {
    _active = true;
    auto& event = EventManager::Get()->emit(SystemManager::Event::SYSTEM_ACTIVATED);
    event.attach<SystemName>(_name);
}

void ISystem::deactivate() {
    _active = false;
    auto& event = EventManager::Get()->emit(SystemManager::Event::SYSTEM_DEACTIVATED);
    event.attach<SystemName>(_name);
}

void SystemManager::runSystem(Group& entities,
                              std::shared_ptr<ISystem> system) {
    if (!system->performOnEntities(entities)) {
        Logger::error("System") << system->_name << " failed";
        Logger::endline();
    }

    if (system->_runOnlyOnce) {
        _systems.erase(system->_name);

        auto it = std::remove(_parallelSystems.begin(),
                              _parallelSystems.begin(), system->_name);

        if (it != _parallelSystems.end())
            _parallelSystems.erase(it, _parallelSystems.end());
        else {
            for (auto& group : _systemGroups) {
                auto it =
                    std::remove(group.begin(), group.end(), system->_name);
                if (it != group.end()) {
                    group.erase(it, group.end());
                    break;
                }
            }
        }
    }
}

void SystemManager::run() {
    std::mutex mutex;
    auto& entities = SceneManager::Get()->getActive().getEntities();
    std::vector<std::thread> handles;

    auto systemThread = [&](std::shared_ptr<ISystem> system) {
        if (system->_active) {
            std::lock_guard lock_(mutex);
            runSystem(entities, system);
        }
    };

    auto systemGroupThread = [&](const std::vector<std::string>& systemNames) {
        std::lock_guard lock_(mutex);
        for (auto& name : systemNames)
            if (_systems[name]->_active) runSystem(entities, _systems[name]);
    };

    for (auto& name : _parallelSystems)
        handles.emplace_back(systemThread, _systems[name]);

    for (auto& systemNames : _systemGroups)
        handles.emplace_back(systemGroupThread, systemNames);

    for (auto& handle : handles) handle.join();
}

void SystemManager::setThreadName(std::thread& handle,
                                  const std::string& name) const {
#if defined(__linux__)
    pthread_setname_np(handle.native_handle(), name.c_str());
#elif defined(_WIN32)
    setThreadDescription(handle.native_handle(),
                         std::wstring(name.begin(), name.end()));
#endif
}

void SystemManager::useFreeSystem(const std::string& systemName) {
    if (_systems.find(systemName) == _systems.end()) {
        Logger::error("System")
            << "System named '" << systemName << "' was not found.";
        Logger::endline();
        return;
    }

    if (isSystemUsed(systemName)) {
        Logger::warn("System")
            << "System '" << systemName << "' is already in use.";
        Logger::endline();
    } else
        _parallelSystems.push_back(systemName);
}

void SystemManager::useSystemSequence(
    const std::vector<std::string>& systemSequence) {
    std::vector<std::string> newGroup;
    for (auto& system : systemSequence) {
        if (isSystemUsed(system)) {
            Logger::warn("System")
                << "System '" << system << "' is already in use.";
            Logger::endline();
        } else
            newGroup.push_back(system);
    }

    if (!newGroup.empty()) _systemGroups.push_back(newGroup);
}

bool SystemManager::isSystemUsed(const std::string& systemName) const {
    if (std::find(_parallelSystems.begin(), _parallelSystems.end(),
                  systemName) != _parallelSystems.end())
        return true;

    for (auto& group : _systemGroups)
        if (std::find(group.begin(), group.end(), systemName) != group.end())
            return true;

    return false;
}

// static
std::shared_ptr<SystemManager> SystemManager::Get() { return createInstance(); }