#include "entity.h"
#include "../components.h"

#include <algorithm>

Group::~Group()
{
    Entity::_destroy(_ids);
    _ids.clear();
}

Entity& Group::create()
{
    auto ret = new Entity;
    _ids.push_back(*ret);

    return *ret;
}

Entity& Group::create(EntityID ID)
{
    auto ret = new Entity(ID);
    _ids.push_back(ID);
    return *ret;
}

Entity& Group::create(const std::string& tag)
{
    auto& e = create();
    e.attach<Component::tag>(tag);
    return e;
}

void Group::erase(EntityID id)
{
    auto it = std::find(_ids.begin(), _ids.end(), id);
    auto& tmp = Entity::get(*it);
    delete &tmp;
    _ids.erase(it);
}

void Group::for_each(_process process)
{
    for (auto& id : _ids)
        process(Entity::get(id));
}

void Group::for_each(_process process, _predicate predicate)
{
    for (auto id : _ids)
    {
        auto& entity = Entity::get(id);
        if (predicate(entity))
            process(entity);
    }
}

std::vector<Entity*> Group::get(_predicate predicate)
{
    std::vector<Entity*> ret;
    for_each([&](Entity& entity){ ret.push_back(&entity); }, predicate);
    return ret;
}

Entity* Group::operator[](const std::string& tag)
{
    for (auto id : _ids)
    {
        auto& entity = Entity::get(id);
        if (entity.has<Component::tag>())
            if (entity.get<Component::tag>().content == tag)
                return &entity;
    }
    return nullptr;
}

Entity* Group::operator[](EntityID ID)
{
    for (auto id : _ids)
    {
        auto& e = Entity::get(ID);
        if (ID == e.id())
            return &e;
    }
    return nullptr;
}

void Group::reorder()
{
    struct compare
    {
        bool operator()(const EntityID& id1, const EntityID& id2) const
        {
            return Entity::get(id1).index < Entity::get(id2).index;
        }
    };
    _ids.sort(compare());
}
