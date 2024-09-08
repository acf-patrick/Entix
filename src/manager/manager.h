/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains base class for managers
 */

#pragma once

#include <memory>

template <typename T>
class Manager {
   protected:
    virtual ~Manager() = default;

    template <typename... TArgs>
    static std::shared_ptr<T> createInstance(TArgs&&... args) {
        static std::shared_ptr<T> instance;

        if (!instance)
            instance = std::shared_ptr<T>(new T(std::forward<TArgs>(args)...),
                                          [](T* ptr) { delete ptr; });

        return instance;
    }
};