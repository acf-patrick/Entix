#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "../logger/logger.h"
#include "../manager/manager.h"

namespace entix {

class ResourceManager : Manager<ResourceManager> {
    struct Data {
        std::mutex mutex;
        void* data;

        Data(void* data) : data(data) {}
        ~Data() { delete data; }
    };

   public:
    template <typename T>
    std::optional<std::pair<std::unique_lock<std::mutex>, T&>> get() {
        const std::string resource = typeid(T).name();

        if (_datas.find(resource) == _datas.end()) return std::nullopt;

        auto record = _datas[resource];
        auto data = static_cast<T*>(record->data);

        return {std::unique_lock(record->mutex), *data};
    }

    template <typename T>
    std::optional<T> read() {
        const std::string resource = typeid(T).name();

        if (_datas.find(resource) == _datas.end()) return std::nullopt;

        auto record = _datas[resource];
        std::lock_guard lock(record->mutex);

        return *(static_cast<T*>(record->data));
    }

    template <typename T, typename... Args>
    ResourceManager& add(Args&&... args) {
        const std::string resource = typeid(T).name();

        auto message = resource + " type has alread been registered";
        assert(_datas.find(resource) != _datas.end() && message.c_str());

        auto data = new T(std::forward<Args>(args)...);
        _datas[resource] = std::make_shared<Data>(data);

        return *this;
    }

    static std::shared_ptr<ResourceManager> Get();

   private:
    std::unordered_map<std::string, std::shared_ptr<Data>> _datas;

    ~ResourceManager() = default;

    friend class Manager<ResourceManager>;
};

}  // namespace entix