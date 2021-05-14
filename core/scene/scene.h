#pragma once

#include "ecs/ecs.h"
#include <vector>
#include <string>

class SceneManager;

// Scene Interface
class Scene
{
protected:

    virtual ~Scene() = default;

// pop the scene from manager if this method return false
    virtual bool update();

    virtual void draw() = 0;

    Scene(const std::string&);

    std::string tag = "Scene";
    Group entities;

    bool active = false;

private:

    Scene* next = nullptr;

friend class SceneManager;
};

class SceneManager
{
public:

    static SceneManager& get();

// Scene methods
    void update();
    void draw();

// container methods
    void add(Scene*);
    
    void remove(Scene*);
    void remove(std::size_t);
    void remove(const std::string&);

    void switch_to(const std::string&);
    void switch_to(std::size_t);

    void swap(const std::string&, const std::string&);
    void swap(std::size_t, std::size_t);

private:

    SceneManager() = default;

    std::vector<Scene*> scenes;

    static SceneManager* instance;
};