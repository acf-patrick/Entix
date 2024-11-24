#pragma once

#include <queue>
#include <variant>

#include "task.h"

namespace entix {
namespace task {

class SequentialTask : public ITask {
    std::queue<std::shared_ptr<ITask>> _tasks;

   public:
    void push(const std::shared_ptr<ITask>& task) override {
        _tasks.push(task);
    }

    bool done() const override { return _tasks.empty(); }

    void run(uint32_t dt) override {
        if (_tasks.empty()) return;

        auto task = _tasks.front();

        task->run(dt);
        if (task->done()) _tasks.pop();
    }
};

}  // namespace task
}  // namespace entix