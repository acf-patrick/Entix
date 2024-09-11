/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains base class for systems
 */

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "../../logger/logger.h"
#include "../../manager/manager.h"
#include "../group/group.h"

class SystemManager;
class Entity;
class IFilter;

class ISystem {
   protected:
    IFilter* _filter;
    std::string _name;
    std::vector<Entity*> _entities;

    bool performOnEntities(Group& entities);

   public:
    ISystem(const std::string& name, IFilter* filter);
    virtual ~ISystem();
    virtual bool run() = 0;

    friend class SystemManager;
};

class SystemManager : Manager<SystemManager> {
    std::map<std::string, std::shared_ptr<ISystem>> _systems;

   public:
    template <typename TSystem>
    void addSystem() {
        auto system = std::make_shared<TSystem>();
        auto message =
            "System with given name " + system->_name + " already exists";
        assert(_systems.find(system->_name) == _systems.end() &&
               message.c_str());
        _systems[system->_name] = system;
    }

    // run systems (TODO : in parallel)
    void run();

    static std::shared_ptr<SystemManager> Get();

    friend class Manager<SystemManager>;
};

#define USE_SYSTEM(System) SystemManager::Get()->addSystem<System>();