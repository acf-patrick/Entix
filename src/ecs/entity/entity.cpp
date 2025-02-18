#include "entity.h"

#include <yaml-cpp/yaml.h>

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#if defined(NDEBUG) && !defined(USE_LIBRARY_AS_STANDALONE)
#include <prefabs.h>
#endif

#include "../../application/application.h"
#include "../../logger/logger.h"
#include "../../task/task_pool.h"
#include "../components.h"

namespace entix {
namespace ecs {

const std::string Entity::PREFABS_FOLDER = "prefabs";
const std::string Entity::FILE_EXTENSION = ".entt";

std::set<EntityID> Entity::takenID;
std::unordered_map<EntityID, Entity*> Entity::instances;
bool Entity::_cleanFlag = false;

Entity::Entity(ComponentManager& componentManager)
    : _id(_generateID(0, true)), _manager(componentManager) {
    _init();
}

Entity::Entity(EntityID id, ComponentManager& componentManager)
    : _id(_generateID(id)), _manager(componentManager) {
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

void Entity::Update(uint32_t dt) {
    if (has<task::TaskPool>()) {
        auto& taskPool = get<task::TaskPool>();
        taskPool.run(dt);
    }

    for (auto& s : _scripts) {
        auto& script = *static_cast<Script*>(s);
        if (script.isEnabled()) script.Update(dt);
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

void Entity::useTemplate(const std::string& templateName) {
    auto& serializer = core::Application::Get().getSerializer();

#ifndef NDEBUG
    Path path(PREFABS_FOLDER);
    path = path / (templateName + FILE_EXTENSION);

    std::ifstream file(path);

    if (!file) {
        Logger::error("Entity")
            << "Failed to load template : '" << path << "' doesn't exist";
        Logger::endline();

        return;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    auto yaml = YAML::Load(ss.str());
#elif !defined(USE_LIBRARY_AS_STANDALONE)
    if (g_prefabs.find(templateName) == g_prefabs.end()) {
        Logger::error("Entity")
            << "Failed to load template : '" << templateName << "' not found";
        Logger::endline();

        return;
    }

    auto yaml = YAML::Load(g_prefabs.at(templateName));
#endif

    serializer.deserializeEntity(yaml, *this);

    Logger::info("Entity") << templateName << " : Template loaded";
    Logger::endline();
}

}  // namespace ecs
}  // namespace entix