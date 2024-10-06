#include "entity.h"

#include <yaml-cpp/yaml.h>

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include "../../application/application.h"
#include "../../logger/logger.h"
#include "../components.h"

namespace entix {
namespace ecs {

std::set<EntityID> Entity::takenID;
std::unordered_map<EntityID, Entity*> Entity::instances;
bool Entity::_cleanFlag = false;

Entity::Entity()
    : _id(_generateID(0, true)), _manager(ComponentManager::Get()) {
    _init();
}

Entity::Entity(EntityID id)
    : _id(_generateID(id)), _manager(ComponentManager::Get()) {
    _init();
}

void Entity::_init() {
    if ((EntityID)instances.size() >= MAX_ENTITIES) {
        Logger::error("Entity") << "Maximum instance number reached!";
        Logger::endline();

        exit(1);
    }
    instances[_id] = this;
}

EntityID Entity::_generateID(EntityID id, bool g) const {
    if (takenID.empty()) srand(time(0));
    while (takenID.find(id) != takenID.end() or g) {
        g = false;
        id = rand() % (1 << 31);
    }
    takenID.emplace(id);
    return id;
}

Entity::~Entity() {
    onDestroy();

    _signature.reset();

    // remove from instances list
    if (!_cleanFlag) instances.erase(_id);

    // signal all components that the entity has been destroyed
    _manager.entityDestroyed(_id);
}

// static
void Entity::Clean() {
    _cleanFlag = true;
    for (auto& [_, entity] : instances) delete entity;
    instances.clear();

    delete ComponentManager::instance;
    ComponentManager::instance = nullptr;
}

// static
Entity* Entity::Get(EntityID id) {
    auto ret = instances[id];
    if (!ret) {
        Logger::warn("Entity")
            << "There is no instance matching ID : " << idToString(id);
        Logger::endline();
    }

    return ret;
}

Entity::operator EntityID() const { return _id; }

EntityID Entity::id() const { return _id; }

// static
std::string Entity::idToString(EntityID id) {
    std::stringstream ss;
    ss << std::hex << id;
    return ss.str();
}

std::string Entity::idAsString() const { return idToString(_id); }

bool Entity::operator==(const Entity& entity) const {
    return _id == entity._id;
}

void Entity::Update() {
    for (auto& s : _scripts) {
        auto& script = *static_cast<Script*>(s);
        if (script.isEnabled()) script.Update();
    }
}

void Entity::Render() {
    for (auto& s : _scripts) {
        auto& script = *static_cast<Script*>(s);
        if (script.isEnabled()) script.Render();
    }
}

void Entity::onDestroy() {
    for (auto& s : _scripts) {
        auto& script = *static_cast<Script*>(s);
        script.onDistach();
    }
    _scripts.clear();
}

int Entity::getIndex() const { return index; }

void Entity::setIndex(unsigned int i) {
    index = i;
    if (has<component::Group>()) get<component::Group>().group->reorder();
}

void Entity::useTemplate(const Path& path) {
    auto& serializer = core::Application::Get().getSerializer();
    std::ifstream file(path);

    if (!file) {
        Logger::error("Entity")
            << "Failed to load template : '" << path << "' doesn't exist";
        Logger::endline();

        return;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    auto n = YAML::Load(ss.str());
    serializer.deserializeEntity(n, *this);

    Logger::info("Entity") << path << " : Template loaded";
    Logger::endline();
}

}  // namespace ecs
}  // namespace entix