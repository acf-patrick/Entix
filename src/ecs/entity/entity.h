#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>

#include "../../path/path.h"
#include "../baseCamera.h"
#include "../baseScript.h"
#include "../component/manager.h"
#include "../defs.h"

class Group;
class EventManager;

/**
 * Create an Entity using Group::create method
 */
class Entity {
   public:
    using Script = BaseScript;
    using Camera = ICamera;

    // make sure to free memory
    static void Clean();

    // get entity with the given ID
    static Entity* Get(EntityID);

    /** construct entity using a template
     * @param fileName
     */
    void useTemplate(const Path&);

    // getter for index property
    int getIndex() const;

    // setter for index property
    void setIndex(unsigned int);

    template <typename T>
    bool has() const {
        return _signature[_manager.getComponentTypeID<T>()];
    }

    template <typename... T>
    bool all_of() const {
        return (has<T>() && ...);
    }

    template <typename... T>
    bool any_of() const {
        return (has<T>() || ...);
    }

    template <typename... T>
    bool none_of() const {
        return (!has<T>() && ...);
    }

    template <typename T>
    T& get() {
        auto component = _manager.getComponent<T>(_id);
        if (!component) {
            auto componentName = typeid(T).name();
            Logger::error("Entity") << "Can not retrieve '" << componentName
                                    << "' on " << idAsString();
            Logger::endline();
        }

        assert(component && "Retrieving non-existent component");

        return *component;
    }

    template <typename... T>
    std::tuple<T&...> retrieve() {
        return std::tuple<T&...>(get<T>()...);
    }

    template <typename T, typename... TArgs>
    T& attach(TArgs&&... args) {
        T* ret = new T(std::forward<TArgs>(args)...);

        _manager.addComponent<T>(_id, ret);
        _signature.set(_manager.getComponentTypeID<T>());

        // Check if attaching script component
        if (std::is_base_of<Script, T>::value) {
            Script* s = (Script*)ret;
            if (s) {
                s->entity = this;
                s->onAttach();
                _scripts.push_back(ret);
            }
        } else if (std::is_base_of<Camera, T>::value) {
            Camera* camera = (Camera*)ret;
            if (camera) {
                camera->entity = this;
                camera->_attachTransform();
            }
        }

        return *ret;
    }

    // Attach component if entity doesn't have
    template <typename T, typename... TArgs>
    T& attachIf(TArgs&&... args) {
        if (has<T>()) return get<T>();
        return attach<T>(args...);
    }

    template <typename T>
    void distach() {
        if (!has<T>()) return;

        if (std::is_base_of<Script, T>::value) {
            auto& tmp = get<T>();
            auto script = static_cast<Script*>((void*)(&tmp));
            script->onDistach();
            _scripts.erase(
                std::remove(_scripts.begin(), _scripts.end(), (void*)script));
        }
        _manager.removeComponent<T>(_id);
        _signature.set(_manager.getComponentTypeID<T>(), false);
    }

    bool operator==(const Entity&) const;

    operator EntityID() const;

    EntityID id() const;

    /**
     * @return string representation of entity's id
     */
    std::string idAsString() const;

    static std::string idToString(EntityID);

    void Update();
    void Render();

    // all we're doing here is distach all of the scripts from this entity
    void onDestroy();

   private:
    Entity();
    Entity(EntityID);
    ~Entity();

    void _init();
    EntityID _generateID(EntityID, bool g = false) const;

   private:
    const EntityID _id;
    Signature _signature;

    ComponentManager& _manager;

    std::vector<void*> _scripts;

    // z-index used when rendering entity
    unsigned int index = 0;

    static std::set<EntityID> takenID;
    static std::unordered_map<EntityID, Entity*> instances;
    static bool _cleanFlag;

    friend class Group;
    friend class EventManager;
};

class Scene;

// Entity Container
class Group {
   public:
    using _process = std::function<void(Entity&)>;
    using _predicate = std::function<bool(const Entity&)>;
    using _compare = std::function<bool(EntityID, EntityID)>;

    // return a list of entities having required components
    std::vector<Entity*> get(_predicate);

    // return a list of entites having required components
    template <typename... TComponents>
    std::vector<Entity*> view() {
        std::vector<Entity*> filtered;
        for (auto id : _ids) {
            auto entity = Entity::Get(id);
            if (entity->all_of<TComponents...>()) filtered.push_back(entity);
        }
        return filtered;
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

    // reorder entites according to the comparator passed as argument
    void reorder(_compare);

   private:
    ~Group();

   private:
    std::list<EntityID> _ids;

    friend class Scene;
};
