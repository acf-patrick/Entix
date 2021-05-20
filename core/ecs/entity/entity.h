#pragma once

#include <list>
#include <queue>
#include <tuple>
#include <memory>
#include <functional>
#include <unordered_map>

#include "../defs.h"
#include "../components.h"
#include "../component/componentManager.h"

class Group;

class Entity
{
public:

// make sure to free memory
    static void clean();

    template<typename T>
    bool has() const
    { return _signature[_manager.getComponentTypeID<T>()]; }

    template<typename... T>
    bool all_of() const
    { return (has<T>() &&...); }

    template<typename... T>
    bool any_of() const
    { return (has<T>() ||...); }

    template<typename... T>
    bool none_of() const
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

    template<typename T>
    bool isScript(T* p)
    {
        auto& script = std::static_pointer_cast<Component::script*>(p);
        return script != nullptr;
    }

    template<typename T, typename... TArgs>
    T& attach(TArgs&&... args)
    {
        using Script = Component::script;

        T* ret = new T(std::forward<TArgs>(args)...);

/*         if (std::is_base_of<Script, T>::value)
        {
            _manager.addComponent<Script>(_id, ret);
            _signature.set(_manager.getComponentTypeID<Script>());
        }
 */        
        {
            _manager.addComponent(_id, ret);
            _signature.set(_manager.getComponentTypeID<T>());
        }

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

    bool operator==  (const Entity&) const;

    operator EntityID() const;
    EntityID id() const;

private:

    Entity();
    ~Entity();

    EntityID _id;
    Signature _signature;

    ComponentManager& _manager;

    static int instance;
    static std::queue<EntityID> availableID;
    static std::unordered_map<EntityID, Entity*> instances;

    static Entity& get(const EntityID&);

friend class Group;
};

// Entity Container
class Group
{
public:

using _process   = std::function<void(Entity&)>;
using _predicate = std::function<bool(const Entity&)>;

    // return a list of entities having required components
    std::vector<const Entity*> get(_predicate);

    void for_each(_process);
    void for_each(_process, _predicate);

    Entity& create();

    void remove(const Entity&);

private:

    std::list<EntityID> _ids;
};
