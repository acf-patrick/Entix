#pragma once

#include <queue>
#include <tuple>
#include <unordered_map>
#include "../defs.h"
#include "../component/componentManager.h"

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
    { return _manager.getComponent<T>(_id); }

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

    operator EntityID();

    static Entity& get(const EntityID&);

private:

    EntityID _id;
    Signature _signature;

    ComponentManager& _manager;

    static int instance;
    static std::queue<EntityID> availableID;
    static std::unordered_map<EntityID, Entity*> instances;

};
