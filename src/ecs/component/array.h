/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Internal class used for component management
 */

#pragma once

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "../../logger/logger.h"
#include "../defs.h"

class IComponentArray {
   public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(EntityID) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
    const std::string name = typeid(T).name();

   public:
    ComponentArray() = default;

    ~ComponentArray() {
        for (auto& component : _componentArray) {
            delete component;
            component = nullptr;
        }
    }

    void insertData(EntityID entity, T* component) {
        if (_entity_index.find(entity) != _entity_index.end()) {
            Logger::warn()
                << name
                << ": Component added to the same entity more than once!";
            Logger::endline();

            return;
        }

        _entity_index[entity] = _size;
        _index_entity[_size] = entity;
        _componentArray[_size] = component;

        ++_size;
    }

    void removeData(EntityID entity) {
        if (_entity_index.find(entity) == _entity_index.end()) {
            Logger::warn() << name << ": Removing non-existent Component!";
            Logger::endline();

            return;
        }

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

    T* getData(EntityID entity) {
        if (_entity_index.find(entity) != _entity_index.end())
            return _componentArray[_entity_index[entity]];
        return nullptr;
    }

    void entityDestroyed(EntityID entity) {
        if (_entity_index.find(entity) != _entity_index.end())
            removeData(entity);
    }

    std::vector<EntityID> getAssociatedEntities() const {
        std::vector<EntityID> entities;
        for (auto& [entity, _] : _entity_index) entities.push_back(entity);
        return entities;
    }

   private:
    size_t _size = 0;
    std::array<T*, MAX_ENTITIES> _componentArray;
    std::unordered_map<EntityID, size_t> _entity_index;
    std::unordered_map<size_t, EntityID> _index_entity;
};