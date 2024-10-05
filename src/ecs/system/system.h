/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains base class for systems
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <thread>
#include <vector>
#include <optional>

#include "../../logger/logger.h"
#include "../../manager/manager.h"
#include "../group/group.h"

class SystemManager;
class SceneManager;
class Entity;
class IFilter;

struct SystemName {
    std::string name;

    SystemName(const std::string& name) : name(name) {}
};

class ISystem {
    using EntityPredicate = std::function<bool(const Entity&)>;

    bool _active = true;
    bool _runOnlyOnce;

   protected:
    IFilter* _filter = nullptr;
    std::string _name;
    std::optional<EntityPredicate> _entityPredicate;
    std::vector<Entity*> _entities;

    bool performOnEntities(Group& entities);

   public:
    ISystem(const std::string& name, bool runOnlyOnce = false);
    ISystem(const std::string& name, IFilter* filter, bool runOnlyOnce = false);
    ISystem(const std::string& name, EntityPredicate&& predicate,
            bool runOnlyOnce = false);

    virtual ~ISystem();

    void activate();
    void deactivate();

    virtual bool run() = 0;

    friend class SystemManager;
};

class SystemManager : Manager<SystemManager> {
    std::map<std::string, std::shared_ptr<ISystem>> _systems;

    // list of group of system names
    std::vector<std::vector<std::string>> _systemGroups;

    // list of system names to be ran in parallel
    std::vector<std::string> _parallelSystems;

    template <typename TSystem>
    void addSystem() {
        auto system = std::make_shared<TSystem>();
        auto message =
            "System with given name " + system->_name + " already exists";
        assert(_systems.find(system->_name) == _systems.end() &&
               message.c_str());
        _systems[system->_name] = system;
    }

    void runSystem(Group& entities, std::shared_ptr<ISystem> system);

    void setThreadName(std::thread&, const std::string&) const;

    bool isSystemUsed(const std::string&) const;

   public:
    struct Event {
        static const std::string SYSTEM_DEACTIVATED;
        static const std::string SYSTEM_ACTIVATED;
    };

    // Register system types
    template <typename... TSystems>
    void add() {
        (addSystem<TSystems>(), ...);
    }

    // Run systems
    void run();

    // Activate systems with given name
    void useFreeSystem(const std::string&);

    // Activate sequence of systems
    void useSystemSequence(const std::vector<std::string>&);

    static std::shared_ptr<SystemManager> Get();

    friend class SceneManager;
    friend class Manager<SystemManager>;
};