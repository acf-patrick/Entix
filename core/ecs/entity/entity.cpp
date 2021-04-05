#include <iostream>
#include "entity.h"

int Entity::instance = 0;
std::queue<std::uint32_t> Entity::availableID;

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
    availableID.pop();

    instance++;
}

Entity::~Entity()
{
    _signature.reset();

    availableID.push(_id);
    instance--;

    _manager.entityDestroyed(_id);
}
