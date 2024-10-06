/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains base class for managers
 */

#pragma once

#include <memory>
#include <vector>

namespace entix {

namespace core {
class Application;
}

class IManager {
   protected:
    virtual ~IManager() = default;

    static void DestroyInstances() {
        for (auto instance : instances) delete instance;
    }

    static std::vector<IManager*> instances;

    friend class core::Application;
};

template <typename T>
class Manager : IManager {
   protected:
    template <typename... TArgs>
    static std::shared_ptr<T> createInstance(TArgs&&... args) {
        static std::shared_ptr<T> self;

        if (!self) {
            auto instance = new T(std::forward<TArgs>(args)...);
            self = std::shared_ptr<T>(instance, [](T* ptr) {});

            instances.push_back(instance);
        }

        return self;
    }
};

}  // namespace entix
