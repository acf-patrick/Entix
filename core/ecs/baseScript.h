#pragma once

class Entity;

// Interface for all scripts
class BaseScript
{
public:
    Entity* entity  = nullptr;

    virtual ~BaseScript() = default;

    void Enable()
    {
        onEnable();
        enabled = true;
    }

    void Disable()
    {
        onDisable();
        enabled = false;
    }

    bool isEnabled()
    { return enabled; }

    // what to do with the entity each frame
    virtual void Update()       {}

    // draw the entity
    virtual void Render()       {}

// supposed to be protected but for some reason (that I don't still get), even if Entity is a friend class of its, Entity can't apparently access to these methods

    // what to do before the script is enabled ?
    virtual void onEnable()     {}

    // what to do before the script is disabled ?
    virtual void onDisable()    {}

    // what to do before the script is attached to an entity
    virtual void onAttach()     {}

    // inverse of onAttach
    virtual void onDetach()     {}

protected:
    bool enabled = true;
};