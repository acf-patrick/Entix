#pragma once

#include <util/vector.h>
#include <string>

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
