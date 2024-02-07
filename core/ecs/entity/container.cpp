#include "entity.h"
#include "../components.h"

#include <algorithm>

Group::~Group()
{
    for (auto id : _ids)
        delete Entity::Get(id);
    _ids.clear();
}

Entity& Group::create()
{
    auto ret = new Entity;
    _ids.push_back(*ret);
    ret->attach<Component::group>(this);
    return *ret;
}

Entity& Group::create(EntityID ID)
{
    auto ret = new Entity(ID);
    _ids.push_back(ID);
    ret->attach<Component::group>(this);
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
    auto tmp = Entity::Get(*it);
    delete tmp;
    _ids.erase(it);
}

void Group::for_each(_process process)
{
    for (auto& id : _ids)
    {
        auto tmp = Entity::Get(id);
        process(*tmp);
    }
}

void Group::for_each(_process process, _predicate predicate)
{
    for (auto id : _ids)
    {
        auto tmp = Entity::Get(id);
        if (predicate(*tmp))
            process(*tmp);
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
        auto entity = Entity::Get(id);
        if (entity->has<Component::tag>())
            if (entity->get<Component::tag>() == tag)
                return entity;
    }
    return nullptr;
}

Entity* Group::operator[](EntityID ID)
{
    for (auto id : _ids)
    {
        auto e = Entity::Get(ID);
        if (ID == e->id())
            return e;
    }
    return nullptr;
}

void Group::reorder()
{
    struct compare
    {
        bool operator()(const EntityID& id1, const EntityID& id2) const
        {
            return Entity::Get(id1)->index < Entity::Get(id2)->index;
        }
    };
    _ids.sort(compare());
}

void Group::reorder(_compare comparator)
{
    _ids.sort(comparator);
}