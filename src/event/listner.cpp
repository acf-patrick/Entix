#include "event.h"

namespace entix {
namespace core {

EventListner::EventListner() {
    manager = EventManager::Get();
    manager->newListner(this);
}

EventListner::~EventListner() { manager->listnerDestroyed(this); }

EventListner& EventListner::listen(const std::string& event,
                                   const WithParameter& callback) {
    Callback callbackRecord;
    callbackRecord.withParameter = true;
    callbackRecord.function = callback;

    callbacks[event] = callbackRecord;
    return *this;
}

EventListner& EventListner::listen(const std::string& event,
                                   const WithoutParameter& callback) {
    Callback callbackRecord;
    callbackRecord.withParameter = false;
    callbackRecord.noParamFunction = callback;

    callbacks[event] = callbackRecord;
    return *this;
}

void EventListner::stopListening(const std::string& event) {
    callbacks.erase(event);
}

void EventListner::removeCallbacks() { callbacks.clear(); }

void EventListner::enable() { enabled = true; }

void EventListner::disable() { enabled = false; }

}  // namespace core
}  // namespace entix
