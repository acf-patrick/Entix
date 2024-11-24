#pragma once

#include <memory>

namespace entix {

namespace task {

class ITask {
   public:
    virtual bool done() const { return false; };

    virtual void push(const std::shared_ptr<ITask>&) {}

    // dt : delta time in miliseconds
    virtual void run(uint32_t dt) = 0;

    virtual ~ITask() = default;
};

}  // namespace task
}  // namespace entix