#include "manager.h"

namespace entix {
namespace ecs {

void ComponentManager::entityDestroyed(EntityID e) {
    for (auto& [_, array] : _componentArrays)
        array->entityDestroyed(e);
}

}  // namespace ecs
}  // namespace entix
