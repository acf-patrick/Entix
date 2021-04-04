#pragma once

#include <bitset>

using EntityID = std::uint32_t;
using ComponentTypeID = std::uint32_t;

const EntityID MAX_ENTITIES  = 500;
const std::uint8_t MAX_COMPONENTS = 30;

using Signature = std::bitset<MAX_COMPONENTS>;
