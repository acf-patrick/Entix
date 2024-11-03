/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Class and constants definition
 */

#pragma once

#include <bitset>
#include <cstdint>

namespace entix {

using EntityID = std::uint32_t;
using ComponentTypeID = std::uint32_t;

const EntityID MAX_ENTITIES = 0x100;
const std::uint8_t MAX_COMPONENTS = 0xff;

using ComponentSignature = std::bitset<MAX_COMPONENTS>;

}  // namespace entix