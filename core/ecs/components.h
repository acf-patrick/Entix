#pragma once

#include <tuple>
#include <string>
#include <util/vector.h>

class Group;
class Entity;

namespace Component {
    
    // ID
    // mostly used to identify Events
    struct tag
    { std::string content; };

    // Space specs
    struct transform
    {
        Vector<double> position;
        VectorF scale;
        double rotation;
    };

    // Entity Container
    // when we create Entity through a group, this components contening the group is attached to the entity
    struct group
    { Group* content; };

    // Script Component
    // Interface for all scripts
    struct script
    {    
        Entity* entity  = nullptr;

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

    // used to manage events
        // EventListner event;

        bool enabled = true;
    
    friend class Entity;
    };

};

