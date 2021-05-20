#pragma once

#include <string>
#include <util/vector.h>

class Group;

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
    public:

        virtual void onDestroy() {};
        virtual void Update() {};
        virtual void Render() {};
    };

};

