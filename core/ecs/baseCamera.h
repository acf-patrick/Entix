#pragma once

class Entity;

class ICamera
{
public:
    Entity* entity;
    virtual ~ICamera() = default;
};