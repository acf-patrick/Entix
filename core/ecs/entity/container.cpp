#include "entity.h"
#include "../components.h"
#include <algorithm>

void Group::emplace(const Entity& entity)
{ 
    _ids.push_back(entity);
    auto& component = Entity::get(entity).attach<GroupComponent>();
    component.content = this;
}

void Group::emplace(Entity&& rhs)
{ 
    _ids.push_back(rhs);
    auto& component = Entity::get(rhs).attach<GroupComponent>();
    component.content = this;
}

void Group::remove(const Entity& entity)
{ _ids.erase(std::remove(_ids.begin(), _ids.end(), entity._id)); }

void Group::for_each(_process process)
{
    for (auto& id : _ids)
        process(Entity::get(id));
}

void Group::for_each(_process process, _predicate predicate)
{
    for (auto& id : _ids)
    {
        auto& entity = Entity::get(id);
        if (predicate(entity))
            process(entity);
    }
}

std::vector<Entity&> Group::get(_predicate predicate)
{
    std::vector<Entity&> ret;
    for_each([&](Entity& entity){ ret.push_back(entity); }, predicate);
    return ret;
}