#pragma once

#include <ecs/ecs.h>
#include <string>
#include <deque>

#include "event/event.h"

class Application;
class SceneManagerType;
class Serializer;

// Scene Interface
class Scene
{
protected:
    virtual ~Scene() = default;

    // pop the scene from manager if this method return false
    virtual bool update();

    virtual void render();

    Scene(const std::string&);

    std::string tag = "Scene";

    // used to manage entities
    Group entities;

    // used to manage events
    EventListner event;

    bool active = true;

friend class Serializer;
friend class SceneManagerType;
};

class SceneManagerType
{
public:
    // switch to next scene
    void next();

    // No more scene left if return false
    bool update();

    void render();

    void push(Scene*);
    
    void remove(std::size_t);
    void remove(const std::string&);

    void switch_to(const std::string&);
    void switch_to(std::size_t);

    void swap(const std::string&, const std::string&);
    void swap(std::size_t, std::size_t);


private:
    std::deque<Scene*> scenes;

    SceneManagerType() = default;
    ~SceneManagerType();

friend class Application;
};

extern SceneManagerType* SceneManager;