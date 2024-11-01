#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>

#include "../../path/path.h"
#include "../base_camera.h"
#include "../base_script.h"
#include "../component/manager.h"
#include "../defs.h"

namespace entix {

namespace core {
class EventManager;
}

namespace ecs {

class Group;

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
    ComponentSignature _signature;

    ComponentManager& _manager;

    std::vector<void*> _scripts;

    // z-index used when rendering entity
    unsigned int index = 0;

    static std::set<EntityID> takenID;
    static std::unordered_map<EntityID, Entity*> instances;
    static bool _cleanFlag;

    friend class Group;
    friend class core::EventManager;
};

}  // namespace ecs
}  // namespace entix
