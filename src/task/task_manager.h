/*#pragma once

#include <memory>
#include <unordered_map>

#include "../manager/manager.h"
#include "task.h"
#include "task_pool.h"

namespace entix {
namespace task {

class TaskManager : Manager<TaskManager> {
    std::unordered_map<std::string, std::shared_ptr<ITask>> _tasks;
    TaskPool _taskPool;

   public:
    template <typename T, typename... TArgs>
    void registerTask(const std::string& taskName, TArgs&&... args) {
        _tasks.try_emplace(taksName, std::make_shared<T>(std::forward<TArgs>(args)...);
    }

    std::shared_ptr<ITask> getTask(const std::string& taskName) {
        if (_tasks.find(taskName) == _tasks.end()) return nullptr;
        return _tasks[taskName];
    }

    static std::shared_ptr<TaskManager> Get() { return createInstance(); }

    friend class Manager<TaskManager>;
};

}  // namespace task
}  // namespace entix
*/