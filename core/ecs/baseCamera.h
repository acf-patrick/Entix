#pragma once

class Entity;

class ICamera
{
public:
    Entity* entity;
    virtual ~ICamera() = default;

protected:
    virtual void _attachTransform() = 0;
friend class Entity;
};