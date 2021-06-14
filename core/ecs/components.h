#pragma once

#include <tuple>
#include <string>
#include <util/vector.h>

class Group;
class Entity;

namespace Component {
    
    // ID
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

    // how to draw the entity
        virtual void Render()       {}

    //protected:
        virtual void onEnable()     {}

        virtual void onDisable()    {}

        virtual void onAttach()     {}

    // call when destroying the entity
        virtual void onDestroy()    {}

    private:
        bool enabled = true;
    
    friend class Entity;
    };

};

