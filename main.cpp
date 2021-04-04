#include <iostream>
#include "ecs/ecs.h"

struct TagComponent
{
    TagComponent(const std::string& s) : tag(s) {}
    std::string tag;
};

struct Position
{
    Position(float _x, float _y) : x(_x), y(_y) {}
    double x, y;
};

int main(int argc, char* argv[])
{
    Entity entity;
    entity.attach<TagComponent>("objet");
    entity.attach<Position>(0, 0);
    std::cout << "Tag : " << entity.get<TagComponent>().tag << std::endl;
    auto& p = entity.get<Position>();
    std::cout << "Position : [" << p.x << ", " << p.y << ']' << std::endl;
    return 0;
}