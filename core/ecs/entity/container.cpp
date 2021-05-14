#include "entity.h"
#include <algorithm>

void Group::emplace(const Entity& entity)
{ _ids.emplace_back(entity); }

void Group::emplace(Entity&& rhs)
{ _ids.emplace_back(rhs); }

void Group::remove(const Entity& entity)
{ _ids.erase(std::find(_ids.begin(), _ids.end(), entity._id)); }

void Group::for_each(process _process)
{
    for (auto& id : _ids)
        _process(Entity::get(id));
}

void Group::for_each(process _process, predicate _predicate)
{
    for (auto& id : _ids)
    {
        auto& entity = Entity::get(id);
        if (_predicate(entity))
            _process(entity);
    }
}