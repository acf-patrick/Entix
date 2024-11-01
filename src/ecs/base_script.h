/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Interface for script components
 */

#pragma once

namespace entix {
namespace ecs {

class Entity;

// Interface for all scripts
class BaseScript {
   public:
    Entity* entity = nullptr;

    virtual ~BaseScript() = default;

    void Enable() {
        onEnable();
        enabled = true;
    }

    void Disable() {
        onDisable();
        enabled = false;
    }

    bool isEnabled() { return enabled; }

    // what to do with the entity each frame
    virtual void Update() {}

    // draw the entity
    virtual void Render() {}

    // supposed to be protected but for some reason (that I don't still get),
    // even if Entity is a friend class of its, Entity can't apparently access
    // to these methods

    // what to do before the script is enabled ?
    virtual void onEnable() {}

    // what to do before the script is disabled ?
    virtual void onDisable() {}

    // what to do before the script is attached to an entity
    virtual void onAttach() {}

    // what to do before the script is distached to an entity
    virtual void onDistach() {}

   protected:
    bool enabled = true;
};

}  // namespace ecs
}  // namespace entix
