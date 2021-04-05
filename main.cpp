#include <iostream>
#include <ecs/ecs.h>

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
    entity.attach<TagComponent>("entity");
    entity.attach<Position>(0, 0);
    auto& p = entity.get<Position>();
    auto [position] = entity.retrieve<Position>();
    std::cout << "method : " << &entity.get<Position>() << std::endl;
    std::cout << "get method : " << &p << std::endl;
    std::cout << "retrieve method : " << &position << std::endl;
    return 0;
}