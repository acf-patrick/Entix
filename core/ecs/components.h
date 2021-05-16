#pragma once

#include <string>
#include <util/vector.h>
#include "entity/entity.h"

namespace Component {
    
    struct tag
    { std::string content; };

    struct transform
    {
        Vector<double> position;
        VectorF scale;
        double rotation;
    };

    struct group
    { Group* content; };

};

