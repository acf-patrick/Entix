#pragma once

#include <string>
#include "entity/entity.h"
#include <util/vector.h>

struct TagComponent
{
    std::string content;
};

struct TransformComponent
{
    Vector<double> position;
    VectorF scale;
    double rotation;
};

struct  GroupComponent
{ 
    Group* content; 
};