#pragma once

#include <memory>
#include <vector>

#include "task.h"

namespace entix {
namespace task {

class AsynchronousTask : public ITask {
    std::vector<std::shared_ptr<ITask>> _tasks;

   public:
    void push(const std::shared_ptr<ITask>& task) override { _tasks.push_back(task); }

    bool done() const override {
        for (auto task : _tasks)
            if (!task->done()) return false;
        return true;
    }

    void run(uint32_t dt) override {
        for (auto task : _tasks) {
            if (!task->done()) task->run(dt);
        }
    }
};

}  // namespace task
}  // namespace entix