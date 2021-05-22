#pragma once

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
    class script
    {
    protected:
        
    public:
        Entity* entity;

        virtual void onAttach() {}
        virtual void onDetach() {}

    // call when destroying the entity
        virtual void onDestroy() {}
        
    // what to do with the entity each frame
        virtual void Update() {}

    // how to draw the entity
        virtual void Render() {}

    friend class Entity;
    };

};

