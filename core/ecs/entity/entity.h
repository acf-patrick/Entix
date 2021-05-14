#pragma once

#include <queue>
#include <tuple>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../defs.h"
#include "../component/componentManager.h"

class Group;

class Entity
{
public:

    Entity();

    ~Entity();

    template<typename T>
    bool has()
    { return _signature[_manager.getComponentTypeID<T>()]; }

    template<typename... T>
    bool all()
    { return (has<T>() &&...); }

    template<typename... T>
    bool any()
    { return (has<T>() ||...); }

    template<typename... T>
    bool none()
    { return (!has<T>() &&...); }

    template<typename T>
    T& get()
    { 
        auto tmp = _manager.getComponent<T>(_id);
        return *tmp;
    }

    template<typename... T>
    std::tuple<T&...> retrieve()
    { return std::tuple<T&...>(get<T>()...); }

    template<typename T, typename... TArgs>
    T& attach(TArgs&&... args)
    {
        T* ret = new T(std::forward<TArgs>(args)...);
        _manager.addComponent(_id, ret);
        _signature.set(_manager.getComponentTypeID<T>());
        return *ret;
    }
    
    template<typename T>
    T& detach()
    {
        T& ret = get<T>();
        _manager.removeComponent<T>(_id);
        _signature.set(_manager.getComponentTypeID<T>(), false);
        return ret;
    }

    bool operator==(const Entity&) const;

    operator EntityID();
    EntityID id();

private:

    EntityID _id;
    Signature _signature;

    ComponentManager& _manager;

    static int instance;
    static std::queue<EntityID> availableID;
    static std::unordered_map<EntityID, Entity*> instances;

    static Entity& get(const EntityID&);

friend class Group;
};

class Group
{
public:

using process   = std::function<void(Entity&)>;
using predicate = std::function<bool(const Entity&)>;

    void for_each(process);
    void for_each(process, predicate);

    void emplace(const Entity&);
    void emplace(Entity&&);

    void remove(const Entity&);

private:

    std::vector<EntityID> _ids;
};