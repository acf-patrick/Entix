#include "manager.h"

namespace entix {
namespace ecs {

ComponentManager* ComponentManager::instance = nullptr;

ComponentManager& ComponentManager::Get() {
    if (!instance) instance = new ComponentManager;
    return *instance;
}

void ComponentManager::entityDestroyed(EntityID e) {
    for (auto& [_, array] : _componentArrays)
        array->entityDestroyed(e);
}

void ComponentManager::onGroupDestroyed() {
    _nextComponentTypeID = 0;
    _componentTypeID.clear();
    _componentArrays.clear();
}

}  // namespace ecs
}  // namespace entix
