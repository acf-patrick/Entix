/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Class used for reading and writing datas
 */

#pragma once

#include <yaml-cpp/yaml.h>

#include "specializations.h"
#include "../ecs/entity/entity.h"
#include "../scene/scene.h"

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

   public:
    Serializer();

    virtual ~Serializer() = default;

    virtual void serialize(Scene*, const std::string& fileName = "");
    virtual Scene* deserialize(const std::string&);

    // Serialize entity
    virtual void serializeEntity(YAML::Emitter&, ecs::Entity&);

    // Fetch system names
    bool deserializeSystems(YAML::Node&, Scene&);

    // Deserialize entity
    virtual void deserializeEntity(YAML::Node&, ecs::Entity&);

    friend class Application;
};

}  // namespace core
}  // namespace entix