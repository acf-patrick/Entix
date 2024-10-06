#include "resource.h"

namespace entix {

std::shared_ptr<ResourceManager> ResourceManager::Get() {
    return createInstance();
}

}