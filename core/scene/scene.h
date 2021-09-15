#pragma once

#include <memory>
#include <string>
#include <deque>

#include "../ecs/entity/entity.h"
#include "../event/event.h"

class Application;
class SceneManagerType;
class Serializer;

// Scene Interface
class Scene
{
public:
    // Save to file
    void save(const std::string& fileName = "");

    // Merge given scene into this scene
    void merge(const Scene&);

    // Set this scene to be active
    void setActive();

protected:
    Scene(const std::string&);
    virtual ~Scene() = default;
    
    // pop the scene from manager if this method return false
    virtual bool update();

    virtual void render();

    std::string tag = "Scene";

    // used to manage entities
    Group entities;

    bool active = true;

friend class Serializer;
friend class SceneManagerType;
};

class SceneManagerType
{
public:
    // Load Scene from file
    void load(const std::string&);

    // Return active scene
    Scene& getActive();

    // Set scene at index to be active
    void setActive(std::size_t);

    // Set scene with the given name to be active
    void setActive(const std::string&);
    
    // Remove scene at index
    void remove(std::size_t);

    // Remove scene with the given name from SceneManager
    void remove(const std::string&);

    // Set next scene to be active
    void next();

    // No more scene left if return false
    bool update();

    // Render active scene
    void render();

private:
    std::deque<Scene*> scenes;

    SceneManagerType() = default;
    ~SceneManagerType();

    void push(Scene*);

friend class Application;
friend class Scene;
};

extern SceneManagerType* SceneManager;