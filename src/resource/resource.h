#pragma once

#include <cassert>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "../logger/logger.h"
#include "../manager/manager.h"

namespace entix {

class ResourceManager : Manager<ResourceManager> {
    struct IData {
        virtual ~IData(){};
    };

    template <typename T>
    struct Data : public IData {
        std::mutex mutex;
        T* data;

        Data(T* data) : data(data) {}
        ~Data() { delete data; }
    };

    template <typename T>
    class Resource {
        T* _data;

       public:
        Resource(T* data) : _data(data) {}

        T* operator->() { return _data; }

        T& operator*() { return *_data; }
    };

   public:
    template <typename T>
    std::optional<std::pair<std::unique_lock<std::mutex>, Resource<T>>> get() {
        const std::string resource = typeid(T).name();

        if (_datas.find(resource) == _datas.end()) return std::nullopt;

        auto record = static_cast<Data<T>*>(_datas[resource].get());

        return std::make_pair(std::unique_lock(record->mutex),
                              Resource(record->data));
    }

    template <typename T>
    std::optional<T> read() {
        const std::string resource = typeid(T).name();

        if (_datas.find(resource) == _datas.end()) return std::nullopt;

        auto record = static_cast<Data<T>*>(_datas[resource].get());
        std::lock_guard lock(record->mutex);

        return *record->data;
    }

    template <typename T, typename... Args>
    ResourceManager& add(Args&&... args) {
        const std::string resource = typeid(T).name();

        if (_datas.find(resource) != _datas.end()) {
            Logger::warn("Resource")
                << resource << " has already been registered";
            Logger::endline();
        } else {
            auto data = new T(std::forward<Args>(args)...);
            _datas[resource] = std::make_shared<Data<T>>(data);
        }

        return *this;
    }

    static std::shared_ptr<ResourceManager> Get();

   private:
    std::unordered_map<std::string, std::shared_ptr<IData>> _datas;

    ~ResourceManager() = default;

    friend class Manager<ResourceManager>;
};

}  // namespace entix