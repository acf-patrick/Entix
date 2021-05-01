#include <iostream>
#include <cassert>
#include "entity.h"

int Entity::instance = 0;
std::queue<std::uint32_t> Entity::availableID;
std::unordered_map<EntityID, Entity*> Entity::instances;

Entity::Entity() : 
    _manager(ComponentManager::get())
{
    if (availableID.empty())
        for (EntityID i=0; i<MAX_ENTITIES; ++i)
            availableID.push(i);

    if ((EntityID)instance >= MAX_ENTITIES)
    {
        std::cerr << "Too many entities created!" << std::endl;
        exit(1);
    }

    _id = availableID.front();
    attach<ID>(_id);

    availableID.pop();
    instances[_id] = this;

    instance++;
}

Entity::~Entity()
{
    _signature.reset();

    instances.erase(_id);
    availableID.push(_id);
    instance--;

    _manager.entityDestroyed(_id);
}

Entity& Entity::get(const EntityID& id)
{
    assert(instances.find(id) != instances.end() && "There is no instance matching with the given ID");

    return *instances[id];
}

Entity::operator EntityID()
{
    return _id;
}
