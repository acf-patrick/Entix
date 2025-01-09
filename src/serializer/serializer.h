/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Class used for reading and writing datas
 */

#pragma once

#include <yaml-cpp/yaml.h>

#include <memory>

#include "../ecs/entity/entity.h"
#include "../scene/scene.h"
#include "../task/task.h"
#include "specializations.h"

namespace entix {
namespace core {

class Application;

/**
 * Serializer class
 * override serialize/desrialize-Entity for more functionality
 * Application::serializer to use unique instance of this class
 */
class Serializer {
    static int _cnt;

    bool _deserializeTask(std::shared_ptr<task::ITask>& task, YAML::Node&,
                          ecs::Entity&);

   public:
    Serializer();

    virtual ~Serializer() = default;

    virtual void serialize(Scene*, const std::string& fileName = "");

    virtual Scene* deserializeRaw(const std::string& yaml);
    virtual Scene* deserialize(const std::string& sceneName);

    // Serialize entity
    virtual void serializeEntity(YAML::Emitter&, ecs::Entity&);

    /**
     * Deserialize with your own task type
     * @param node task node
     * @param entity entity the task pool will be attached to
     */
    virtual std::shared_ptr<task::ITask> deserializeTask(
        const YAML::Node& node, ecs::Entity& entity);

    bool deserializeTasks(YAML::Node&, ecs::Entity&);

    // Fetch system names
    bool deserializeSystems(YAML::Node&, Scene&);

    // Deserialize entity
    virtual void deserializeEntity(YAML::Node&, ecs::Entity&);

    friend class Application;
};

}  // namespace core
}  // namespace entix