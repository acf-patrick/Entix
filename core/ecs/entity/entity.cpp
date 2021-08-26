#include <iostream>
#include <cassert>
#include <cstdlib>

#include "entity.h"
#include "../components.h"
#include <set>

std::set<EntityID> Entity::takenID;
std::unordered_map<EntityID, Entity*> Entity::instances;

Entity::Entity() : 
    _id(_generateID(0, true)),
    _manager(ComponentManager::get())
{
    _init();
}

Entity::Entity(EntityID id) : 
    _id(_generateID(id)),
    _manager(ComponentManager::get())
{
    _init();
}

void Entity::_init()
{
    if ((EntityID)instances.size() >= MAX_ENTITIES)
    {
        std::cerr << "Too many entities created!" << std::endl;
        exit(1);
    }
    instances[_id] = this;
}

EntityID Entity::_generateID(EntityID id, bool g) const
{
    if (takenID.empty())
        srand(time(0));
    while (takenID.find(id) != takenID.end() or g)
    {
        g = false;
        id = rand()%(1<<31);
    }
    takenID.emplace(id);
    return id;
}

Entity::~Entity()
{
    onDestroy();

    _signature.reset();

    // remove from its group
    if (has<Component::group>())
    {
        auto& group = *get<Component::group>().content;
        group.erase(_id);
    }

    // remove from instances list
    instances.erase(_id);

    // signal all components that the entity has been destroyed
    _manager.entityDestroyed(_id);
}

void Entity::clean()
{
    for (auto& [_, entity] : instances)
        delete entity;
    instances.clear();

    delete ComponentManager::instance;
    ComponentManager::instance = nullptr;
}

Entity& Entity::get(EntityID id)
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

void Entity::Update()
{
    for (auto& s : _scripts)
    {
        auto& script = *static_cast<Script*>(s);
        if (script.isEnabled())
            script.Update();
    }
}

void Entity::Render()
{
    for (auto& s : _scripts)
    {
        auto& script = *static_cast<Script*>(s);
        if (script.isEnabled())
            script.Render();
    }
}

void Entity::onDestroy()
{
    for (auto& s : _scripts)
    {
        auto& script = *static_cast<Script*>(s);
        script.onDetach();
    }
    _scripts.clear();
}

int Entity::getIndex() const
{
    return index;
}

void Entity::setIndex(unsigned int i)
{
    index = i;
    if (has<Component::group>())
        get<Component::group>().content->reorder();
}