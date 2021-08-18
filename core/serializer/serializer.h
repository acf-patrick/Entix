#pragma once

#include <yaml-cpp/yaml.h>

#include "../ecs/components.h"
#include "../scene/scene.h"

YAML::Emitter& operator<< (YAML::Emitter&, const Component::tag&);
YAML::Emitter& operator<< (YAML::Emitter&, const Component::transform&);
YAML::Emitter& operator<< (YAML::Emitter&, const Component::sprite&);