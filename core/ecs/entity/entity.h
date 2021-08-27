#pragma once

#include <set>
#include <list>
#include <tuple>
#include <memory>
#include <functional>
#include <unordered_map>

#include "../defs.h"
#include "../baseScript.h"
#include "../baseCamera.h"
#include "../component/manager.h"

class Group;
class EventManagerType;

// Create an Entity using Group::create method
class Entity
{
public:
using Script = BaseScript;
using Camera = ICamera;

    // make sure to free memory
    static void clean();

    // get entity with the given ID
    static Entity& get(EntityID);

    // getter for index property
    int  getIndex() const;

    // setter for index property
    void setIndex(unsigned int);

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
        else if (std::is_base_of<Camera, T>::value)
        {
            Camera* camera = (Camera*)ret;
            if (camera)
            {
                camera->entity = this;
                camera->_attachTransform();
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
            auto& tmp = get<T>();
            void* script = &tmp;
            ((Script*)script)->onDetach();

            int i = 0, len = _scripts.size();
            while (_scripts[i] != script)
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

    void Update();
    void Render();
    
// all we're doing here is detach all of the scripts from this entity
    void onDestroy();

private:
    Entity();
    Entity(EntityID);
    ~Entity();

    void _init();
    EntityID _generateID(EntityID, bool g = false) const;

    static void _destroy(const std::list<EntityID>&);

private:
    const EntityID _id;
    Signature _signature;

    ComponentManager& _manager;

    std::vector<void*> _scripts;

    // z-index used when rendering entity
    unsigned int index = 0;

    static std::set<EntityID> takenID;
    static std::unordered_map<EntityID, Entity*> instances;

friend class Group;
friend class EventManagerType;
};

// Entity Container
class Group
{
public:

using _process   = std::function<void(Entity&)>;
using _predicate = std::function<bool(const Entity&)>;

    // return a list of entities having required components
    std::vector<Entity*> get(_predicate);

    // return a list of entites having required components
    template<typename... TComponents>
    std::vector<Entity*> view()
    {
        std::vector<Entity*> ret;
        for (auto id : _ids)
        {
            auto& e = Entity::get(id);
            if (e.all_of<TComponents...>())
                ret.push_back(&e);
        }
        return ret;
    }

    // retrieve by tag
    // return null pointer if not found
    Entity* operator[](const std::string&);

    // retrieve by ID
    // return null pointer if not found
    Entity* operator[](EntityID);

    // apply a callback to entities in this group
    void for_each(_process);
    
    // apply a callback to entities of this group, respecting 
    // condition defined by the predicate function
    void for_each(_process, _predicate);

    // create an entity
    Entity& create();

    // create an entity and associate with an ID
    Entity& create(EntityID);

    // create an entity and attach a tag component to it
    Entity& create(const std::string&);

    // Remove Entity from this group without destroy
    void erase(EntityID);

    // reorder entities according to entity index
    void reorder();

    ~Group();

private:
    std::list<EntityID> _ids;
};
