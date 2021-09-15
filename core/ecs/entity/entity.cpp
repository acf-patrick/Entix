#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <set>

#include <yaml-cpp/yaml.h>

#include "entity.h"
#include "../components.h"
#include "../../application/application.h" 

std::set<EntityID> Entity::takenID;
std::unordered_map<EntityID, Entity*> Entity::instances;
bool Entity::_cleanFlag = false;

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
        std::cerr << "Entity : Maximum instance number reached!" << std::endl;
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

    // remove from instances list
    if (!_cleanFlag)
        instances.erase(_id);
    
    // signal all components that the entity has been destroyed
    _manager.entityDestroyed(_id);
}

void Entity::clean()
{
    _cleanFlag = true;
    for (auto& [_, entity] : instances)
        delete entity;
    instances.clear();

    delete ComponentManager::instance;
    ComponentManager::instance = nullptr;
}

Entity* Entity::get(EntityID id)
{
    auto ret = instances[id];
    if (!ret)
        std::cerr << "There is no instance matching with the given ID" << std::endl;
    return ret;
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

void Entity::useTemplate(const std::string& fileName)
{
    auto& s = *Application::serializer;
    std::ifstream file(fileName);
    if (!file)
    {
        std::cerr << "Failed to load template : " << fileName << " doesn't exist" << std::endl;
        return;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    auto n = YAML::Load(ss.str());
    s.deserializeEntity(n, *this);
    std::cout << fileName << " : Template loaded" << std::endl;
}