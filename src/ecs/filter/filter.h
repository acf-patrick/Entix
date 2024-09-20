/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains interface for entity filters
 */

#pragma once

#include "../defs.h"
#include "../entity/entity.h"

class IFilter {
   public:
    virtual ~IFilter() = default;
    virtual bool filter(EntityID) const = 0;
};

template <typename... TComponents>
class AllOf : public IFilter {
   public:
    AllOf() = default;
    bool filter(EntityID id) const override {
        auto entity = Entity::Get(id);
        if (entity) return entity->all_of<TComponents...>();
        return false;
    }
};

template <typename... TComponents>
class NoneOf : public IFilter {
   public:
    NoneOf() = default;

    

    bool filterEntity(EntityID id) const override {
        auto entity = Entity::Get(id);
        if (entity) return entity->none_of<TComponents...>();
        return false;
    }
};

template <typename... TComponents>
class AnyOf : public IFilter {
   public:
    AnyOf() = default;
    bool filter(EntityID id) const override {
        auto entity = Entity::Get(id);
        if (entity) return entity->any_of<TComponents...>();
        return false;
    }
};

template <typename T>
class Has : public IFilter {
   public:
    Has() = default;
    bool filter(EntityID id) const override {
        auto entity = Entity::Get(id);
        if (entity) return entity->has<T>();
        return false;
    }
};