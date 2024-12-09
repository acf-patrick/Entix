#pragma once

#include <memory>

#include "task.h"

namespace entix {
namespace task {

class TaskPool {
    std::shared_ptr<ITask> _root;

   public:
    TaskPool(const std::shared_ptr<ITask>& root) : _root(root) {}

    void run(uint32_t dt) {
        if (_root)
            if (!_root->done()) _root->run(dt);
    }
};

}  // namespace task
}  // namespace entix