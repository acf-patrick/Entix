#include "group.h"

#include <algorithm>

#include "../components.h"
#include "../entity/entity.h"
#include "../filter/filter.h"

namespace entix {
namespace ecs {

Group::Group() : _componentManager(ComponentManager::Get()) {}

Group::~Group() {
    for (auto id : _ids) delete Entity::Get(id);
    _ids.clear();
}

Entity& Group::create() {
    auto ret = new Entity;
    _ids.push_back(*ret);
    ret->attach<component::Group>(this);
    return *ret;
}

Entity& Group::create(EntityID ID) {
    auto ret = new Entity(ID);
    _ids.push_back(ID);
    ret->attach<component::Group>(this);
    return *ret;
}

Entity& Group::create(const std::string& tag) {
    auto& e = create();
    e.attach<component::Tag>(tag);
    return e;
}

void Group::erase(EntityID id) {
    auto it = std::find(_ids.begin(), _ids.end(), id);
    auto tmp = Entity::Get(*it);
    delete tmp;
    _ids.erase(it);
}

void Group::for_each(_process process) {
    for (auto& id : _ids) {
        auto tmp = Entity::Get(id);
        process(*tmp);
    }
}

void Group::for_each(_process process, _predicate predicate) {
    for (auto id : _ids) {
        auto tmp = Entity::Get(id);
        if (predicate(*tmp)) process(*tmp);
    }
}

std::vector<Entity*> Group::get(_predicate predicate) {
    std::vector<Entity*> ret;
    for_each([&](Entity& entity) { ret.push_back(&entity); }, predicate);
    return ret;
}

Entity* Group::operator[](const std::string& tag) {
    for (auto id : _ids) {
        if (auto entity = Entity::Get(id); entity)
            if (entity->has<component::Tag>())
                if (entity->get<component::Tag>() == tag) return entity;
    }
    return nullptr;
}

Entity* Group::operator[](EntityID ID) {
    for (auto id : _ids) {
        auto e = Entity::Get(ID);
        if (ID == e->id()) return e;
    }
    return nullptr;
}

void Group::reorder() {
    struct compare {
        bool operator()(const EntityID& id1, const EntityID& id2) const {
            return Entity::Get(id1)->index < Entity::Get(id2)->index;
        }
    };
    _ids.sort(compare());
}

void Group::reorder(_compare comparator) { _ids.sort(comparator); }

std::vector<Entity*> Group::view(const IFilter& filter) {
    return filter.filter(*this);
}

std::vector<Entity*> Group::getEntities() {
    std::vector<Entity*> entities;
    for (auto id : _ids) entities.push_back(Entity::Get(id));
    return entities;
}

template <typename T>
std::vector<Entity*> Group::getEntitiesWith() {
    auto ids = _componentManager.getEntitiesWith<T>();
    std::vector<Entity*> entities(ids.size());

    for (size_t i = 0; i < ids.size(); ++i) entities[i] = Entity::Get(ids[i]);

    return entities;
}

template <typename... Components>
std::vector<Entity*> Group::getEntitiesWithAnyOf() {
    auto ids = _componentManager.getEntitiesWithAnyOf<Components...>();
    std::vector<Entity*> entities(ids.size());

    for (size_t i = 0; i < ids.size(); ++i) entities[i] = Entity::Get(ids[i]);

    return entities;
}

template <typename... Components>
std::vector<Entity*> Group::getEntitiesWithAllOf() {
    auto ids = _componentManager.getEntitiesWithAllOf<Components...>();
    std::vector<Entity*> entities(ids.size());

    for (size_t i = 0; i < ids.size(); ++i) entities[i] = Entity::Get(ids[i]);

    return entities;
}

template <typename... Components>
std::vector<Entity*> Group::getEntitiesWithNoneOf() {
    return get(
        [](const Entity& entity) { return entity.none_of<Components...>(); });
}

}  // namespace ecs
}  // namespace entix
