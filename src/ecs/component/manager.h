/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Main component class composer
 */

#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>

#include "../defs.h"
#include "array.h"

namespace entix {
namespace ecs {

class Entity;
class Group;

class ComponentManager {
   private:
    static ComponentManager& Get();

    void entityDestroyed(EntityID);

    template <typename T>
    ComponentTypeID getComponentTypeID() {
        const char* typeName = typeid(T).name();

        if (_componentTypeID.find(typeName) == _componentTypeID.end())
            registerComponent<T>();

        return _componentTypeID[typeName];
    }

    template <typename T>
    void addComponent(EntityID e, T* component) {
        getComponentArray<T>()->insertData(e, component);
    }

    template <typename T>
    void removeComponent(EntityID e) {
        getComponentArray<T>()->removeData(e);
    }

    template <typename T>
    T* getComponent(EntityID e) {
        return getComponentArray<T>()->getData(e);
    }

    template <typename T>
    void registerComponent() {
        const char* typeName = typeid(T).name();

        if (_componentTypeID.find(typeName) != std::end(_componentTypeID))
            return;

        _componentTypeID.insert({typeName, _nextComponentTypeID});
        _componentArrays.insert(
            {typeName, std::make_shared<ComponentArray<T>>()});

        ++_nextComponentTypeID;
    }

    template <typename T>
    std::shared_ptr<ComponentArray<T>> getComponentArray() {
        const char* typeName = typeid(T).name();

        if (_componentTypeID.find(typeName) == std::end(_componentTypeID))
            registerComponent<T>();

        return std::static_pointer_cast<ComponentArray<T>>(
            _componentArrays[typeName]);
    }

    template <typename T>
    std::vector<EntityID> getEntitiesWith() {
        return getComponentArray<T>()->getAssociatedEntities();
    }

    template <typename... Components>
    std::vector<EntityID> getEntitiesWithAnyOf() {
        std::set<EntityID> entities;
        (cumulateEntitiesWithGivenComponentTypeIntoList<Components>(entities),
         ...);
        return std::vector<EntityID>(entities.begin(), entities.end());
    }

    template <typename... Components>
    std::vector<EntityID> getEntitiesWithAllOf() {
        std::set<EntityID> entities;
        (cumulateEntitiesWithGivenComponentTypeIntoListIfHas<Components>(
             entities),
         ...);
        return std::vector<EntityID>(entities.begin(), entities.end());
    }

    template <typename T>
    void cumulateEntitiesWithGivenComponentTypeIntoList(
        std::set<EntityID>& entities) const {
        auto entitiesWithComponent = getEntitiesWith<T>();
        std::copy(entitiesWithComponent.begin(), entitiesWithComponent.end(),
                  std::inserter(entities, entities.end()));
    }

    template <typename T>
    void cumulateEntitiesWithGivenComponentTypeIntoListIfHas(
        std::set<EntityID>& entities) const {
        auto entitiesWithComponent = getEntitiesWith<T>();
        std::shared_ptr<IComponentArray> componentArray =
            getComponentArray<T>();

        std::copy_if(entitiesWithComponent.begin(), entitiesWithComponent.end(),
                     std::inserter(entities, entities.end()), [&](EntityID id) {
                         return componentArray->hasRecordAttachedTo(id);
                     });
    }

   private:
    ComponentManager() : _nextComponentTypeID(0) { instance = this; }

    void onGroupDestroyed();

    static ComponentManager* instance;

    std::unordered_map<const char*, ComponentTypeID> _componentTypeID;
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>>
        _componentArrays;
    ComponentTypeID _nextComponentTypeID;

    friend class Entity;
    friend class Group;
};

}  // namespace ecs
}  // namespace entix
