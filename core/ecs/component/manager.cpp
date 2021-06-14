#include "manager.h"

ComponentManager* ComponentManager::instance = nullptr;

ComponentManager& ComponentManager::get()
{
	if (!instance)
		instance = new ComponentManager;
	return *instance;
}

void ComponentManager::entityDestroyed(EntityID e)
{
	for (auto& [_, array] : _componentArrays)
		array->entityDestroyed(e);
}
