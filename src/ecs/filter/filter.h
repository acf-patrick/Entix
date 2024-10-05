/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains interface for entity filters
 */

#pragma once

#include "../defs.h"
#include "../entity/entity.h"
#include "../group/group.h"

class IFilter {
   public:
    virtual ~IFilter() = default;
    virtual std::vector<Entity*> filter(Group&) const = 0;
};

namespace Filter {

template <typename... Components>
class AllOf : public IFilter {
   public:
    AllOf() = default;
    std::vector<Entity*> filter(Group& group) const override {
        return group.getEntitiesWithAllOf<Components...>();
    }
};

template <typename... Components>
class AnyOf : public IFilter {
   public:
    AnyOf() = default;
    std::vector<Entity*> filter(Group& group) const override {
        return group.getEntitiesWithAnyOf<Components...>();
    }
};

template <typename... Components>
class NoneOf : public IFilter {
   public:
    NoneOf() = default;
    std::vector<Entity*> filter(Group& group) const override {
        return group.getEntitiesWithNoneOf<Components...>();
    }
};

template <typename Component>
class With : public IFilter {
   public:
    With() = default;
    std::vector<Entity*> filter(Group& group) const override {
        return group.getEntitiesWith<Component>();
    }
};

}