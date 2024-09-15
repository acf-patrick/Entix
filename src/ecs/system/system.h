/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains base class for systems
 */

#pragma once

#include <map>
#include <memory>
#include <thread>
#include <vector>

#include "../../logger/logger.h"
#include "../../manager/manager.h"
#include "../group/group.h"

class SystemManager;
class Entity;
class IFilter;

class ISystem {
    bool _runOnlyOnce;

   protected:
    IFilter* _filter = nullptr;
    std::string _name;
    std::vector<Entity*> _entities;

    bool performOnEntities(Group& entities);

   public:
    ISystem(const std::string& name, bool runOnlyOnce = false);
    ISystem(const std::string& name, IFilter* filter, bool runOnlyOnce = false);

    virtual ~ISystem();
    virtual bool run() = 0;

    friend class SystemManager;
};

class SystemManager : Manager<SystemManager> {
    std::map<std::string, std::shared_ptr<ISystem>> _systems;

    // list of group of system names
    std::vector<std::vector<std::string>> _systemGroups;

    // list of system names to be ran in parallel
    std::vector<std::string> _parallelSystems;

    // lastly added systems
    std::vector<std::string> _lastNamesAdded;

    template <typename TSystem>
    void addSystem() {
        auto system = std::make_shared<TSystem>();
        auto message =
            "System with given name " + system->_name + " already exists";
        assert(_systems.find(system->_name) == _systems.end() &&
               message.c_str());
        _systems[system->_name] = system;

        _lastNamesAdded.push_back(system->_name);
    }

    void runSystem(Group& entities, std::shared_ptr<ISystem> system);

    void setThreadName(std::thread&, const std::string&) const;

   public:
    // Systems registered with this method will be ran in parallel
    template <typename... TSystems>
    void add() {
        _lastNamesAdded.clear();
        (addSystem<TSystems>(), ...);

        _parallelSystems.insert(_parallelSystems.end(), _lastNamesAdded.begin(),
                                _lastNamesAdded.end());
    }

    // Systems register with this method will be ran sequentially 
    template <typename... TSystems>
    void addSequence() {
        _lastNamesAdded.clear();
        (addSystem<TSystems>(), ...);

        // create new group
        _systemGroups.push_back(_lastNamesAdded);
    }

    // Run systems (TODO : in parallel)
    void run();

    static std::shared_ptr<SystemManager> Get();

    friend class Manager<SystemManager>;
};