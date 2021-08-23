#pragma once

#include <ecs/ecs.h>
#include <string>
#include <deque>

#include "event/event.h"

class SceneManager;
class Serializer;

// Scene Interface
class Scene
{
protected:
    virtual ~Scene() = default;

    // pop the scene from manager if this method return false
    virtual bool update();

    Scene(const std::string&);

    std::string tag = "Scene";

    // used to manage entities
    Group entities;

    // used to manage events
    EventListner event;

friend class Serializer;
friend class SceneManager;
};

class SceneManager
{
public:

    static SceneManager& get();

    static void clean();

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

    SceneManager() = default;
    ~SceneManager();

    std::deque<Scene*> scenes;

    static SceneManager* instance;
};