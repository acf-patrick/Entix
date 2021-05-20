#include "entity.h"
#include "../components.h"
#include <algorithm>

Entity& Group::create()
{
    auto ret = new Entity;
    _ids.push_back(*ret);
    return *ret;
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

std::vector<const Entity*> Group::get(_predicate predicate)
{
    std::vector<const Entity*> ret;
    for_each([&](Entity& entity){ ret.push_back(&entity); }, predicate);
    return ret;
}