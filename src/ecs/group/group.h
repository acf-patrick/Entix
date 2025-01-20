#pragma once

#include <functional>
#include <list>
#include <vector>

#include "../component/manager.h"
#include "../defs.h"

namespace entix {

namespace core {
class Scene;
}

namespace ecs {

class IFilter;
class Entity;

// Entity Container
class Group {
   public:
    using _process = std::function<void(Entity&)>;
    using _predicate = std::function<bool(const Entity&)>;
    using _compare = std::function<bool(EntityID, EntityID)>;

    // return a list of entities conforming to predicate
    std::vector<Entity*> get(_predicate);

    // return a list of entites having required components
    std::vector<Entity*> view(const IFilter& filter);

    // return all entities from this group
    std::vector<Entity*> getEntities();

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

    template <typename T>
    std::vector<Entity*> getEntitiesWith();

    template <typename... Components>
    std::vector<Entity*> getEntitiesWithAnyOf();

    template <typename... Components>
    std::vector<Entity*> getEntitiesWithAllOf();

    template <typename... Components>
    std::vector<Entity*> getEntitiesWithNoneOf();

   private:
    Group() = default;
    ~Group();

   private:
    std::list<EntityID> _ids;
    ComponentManager _componentManager;

    friend class core::Scene;
};

}  // namespace ecs
}  // namespace entix