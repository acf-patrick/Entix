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
    template <typename Type>
    struct Counter {
        int counter = 0;
    };

   protected:
    template <typename... TArgs>
    static std::shared_ptr<T> createInstance(TArgs&&... args) {
        static std::shared_ptr<T> self;
        static Counter<T> instanciation;

        if (!self) {
            if (instanciation.counter > 0) return nullptr;

            auto instance = new T(std::forward<TArgs>(args)...);
            instanciation.counter++;
            self = std::shared_ptr<T>(instance, [](T* ptr) {});

            instances.push_back(instance);
        }

        return self;
    }
};

}  // namespace entix
