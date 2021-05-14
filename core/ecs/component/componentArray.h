#pragma once

#include <array>
#include <cassert>
#include <unordered_map>
#include "../defs.h"

class IComponentArray
{
public:

    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(EntityID) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:

    ComponentArray() = default;

    ~ComponentArray()
    {
        for (auto& component : _componentArray)
        {
            delete component;
            component = nullptr;
        }
    }

    void insertData(EntityID entity, T* component)
    {
        assert(_entity_index.find(entity) == std::end(_entity_index) && "Component added to the same entity more than once!");

        _entity_index[entity] = _size;
        _index_entity[_size] = entity;
        _componentArray[_size] = component;

        ++_size;
    }

    void removeData(EntityID entity)
    {
        assert(_entity_index.find(entity) != std::end(_entity_index) && "Removing non-existent component!");

        size_t entityIndex = _entity_index[entity];
        size_t lastIndex = _size - 1;
        
        delete _componentArray[entityIndex];
        _componentArray[entityIndex] = _componentArray[lastIndex];
        _componentArray[lastIndex] = nullptr;

        EntityID lastEntity = _index_entity[lastIndex];
        _entity_index[lastEntity] = entityIndex;
        _index_entity[entityIndex] = lastEntity;

        _entity_index.erase(entity);
        _index_entity.erase(entityIndex);

        --_size;
    }

    T* getData(EntityID entity)
    {
        assert(_entity_index.find(entity) != std::end(_entity_index) && "Retrieving non-existent component");

        return _componentArray[_entity_index[entity]];
    }

    void entityDestroyed(EntityID entity)
    {
        if (_entity_index.find(entity) != std::end(_entity_index))
            removeData(entity);
    }

private:
    
    size_t _size = 0;
    std::array<T*, MAX_ENTITIES> _componentArray;
    std::unordered_map<EntityID, size_t> _entity_index;
    std::unordered_map<EntityID, size_t> _index_entity;
};