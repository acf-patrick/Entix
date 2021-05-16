#include <iostream>
#include <cassert>
#include "entity.h"
#include "../components.h"

int Entity::instance = 0;
std::queue<std::uint32_t> Entity::availableID;
std::unordered_map<EntityID, Entity*> Entity::instances;

Entity::Entity() : 
    _manager(ComponentManager::get())
{
    if (availableID.empty())
        for (EntityID i=0; i < MAX_ENTITIES; ++i)
            availableID.push(i);

    if ((EntityID)instance >= MAX_ENTITIES)
    {
        std::cerr << "Too many entities created!" << std::endl;
        exit(1);
    }

    _id = availableID.front();

    availableID.pop();
    instances[_id] = this;

    instance++;
}

Entity::Entity(const Entity& entity) : 
    _manager(ComponentManager::get())
{
    _id = entity._id;
    _signature = entity._signature;
}

Entity::Entity(Entity&& rhs) : 
    _manager(ComponentManager::get())
{
    _id = rhs._id;
    _signature = rhs._signature;
}

Entity::~Entity()
{
    _signature.reset();

// remove from its group
    if (has<Component::group>())
    {
        auto* group = get<Component::group>().content;
        group->remove(*this);
    }

// remove from instances list
    instances.erase(_id);
    availableID.push(_id);
    instance--;

// signal all components that the entity has been destroyed
    _manager.entityDestroyed(_id);
}

void Entity::clean()
{
    for (auto& pair : instances)
        delete pair.second;
    instances.clear();

    delete ComponentManager::instance;
    ComponentManager::instance = nullptr;
}

Entity& Entity::get(const EntityID& id)
{
    assert(instances.find(id) != instances.end() && "There is no instance matching with the given ID");

    return *instances[id];
}

Entity::operator EntityID() const
{ return _id; }

EntityID Entity::id() const
{ return _id; }

bool Entity::operator==(const Entity& entity) const
{ return _id == entity._id; }

Entity& Entity::operator=(const Entity& entity)
{
    _id = entity._id;
    _signature = entity._signature;
    return *this;
}

Entity& Entity::operator=(Entity&& rhs)
{
    _id = rhs._id;
    _signature = rhs._signature;
    return *this;
}