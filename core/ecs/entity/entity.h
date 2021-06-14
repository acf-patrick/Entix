#pragma once

#include <list>
#include <queue>
#include <tuple>
#include <memory>
#include <functional>
#include <unordered_map>

#include "../defs.h"
#include "../components.h"
#include "../component/manager.h"

class Group;
class EventManager;

class Entity
{
public:
using Script = Component::script;

// make sure to free memory
    static void clean();

// get entity with the given ID
    static Entity& get(EntityID);

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

    template<typename T, typename... TArgs>
    T& attach(TArgs&&... args)
    {
        T* ret = new T(std::forward<TArgs>(args)...);
        
        _manager.addComponent<T>(_id, ret);
        _signature.set(_manager.getComponentTypeID<T>());

        if (std::is_base_of<Script, T>::value)
        {
            Script* s = (Script*)ret;
            if (s)
            {
                s->entity = this;
                s->onAttach();
                _scripts.push_back(ret);
            }
        }

        return *ret;
    }
    
    template<typename T>
    void detach()
    {
        if (!has<T>())
            return;

        if (std::is_base_of<Script, T>::value)
        {
            auto& s = get<T>();
            int i = 0, len = _scripts.size();
            while (_scripts[i] != &s)
                ++i;
            if (i < len)
            {
                std::swap(_scripts[i], _scripts[len-1]);
                _scripts.pop_back();
            }
        }
        _manager.removeComponent<T>(_id);
        _signature.set(_manager.getComponentTypeID<T>(), false);
    }

    bool operator==  (const Entity&) const;

    operator EntityID() const;
    EntityID id() const;

public:
    void Update()
    {
        for (auto& s : _scripts)
        {
            auto& script = *static_cast<Script*>(s);
            if (script.isEnabled())
                script.Update();
        }
    }

    void Render()
    {
        for (auto& s : _scripts)
        {
            auto& script = *static_cast<Script*>(s);
            if (script.isEnabled())
                script.Render();
        }
    }

    void onDestroy()
    {
        for (auto& s : _scripts)
        {
            auto& script = *static_cast<Script*>(s);
            script.onDestroy();
        }
    }

private:
    Entity();
    ~Entity();

    EntityID _id;
    Signature _signature;

    ComponentManager& _manager;

    std::vector<void*> _scripts;

private:

    static int instance;
    static std::queue<EntityID> availableID;
    static std::unordered_map<EntityID, Entity*> instances;

friend class Group;
friend class EventManager;
};

// Entity Container
class Group
{
public:

using _process   = std::function<void(Entity&)>;
using _predicate = std::function<bool(const Entity&)>;

    // return a list of entities having required components
    std::vector<const Entity*> get(_predicate);
    // retrieve by tag
    Entity* operator[](const std::string&);

    void for_each(_process);
    void for_each(_process, _predicate);

    Entity& create();

// The entity is immediatly destroyed after calling this function
    void remove(EntityID);

private:

    std::list<EntityID> _ids;
};
