#include "scene.h"
#include <cassert>
#include <algorithm>

SceneManager* SceneManager::instance = nullptr;
SceneManager& SceneManager::get()
{
    if (!instance)
        instance = new SceneManager;
    return *instance;
}
void SceneManager::clean()
{
    delete instance;
    instance = nullptr;
}

SceneManager::~SceneManager()
{
    for (auto& scene : scenes)
    {
        delete scene;
        scene = nullptr;
    }
    scenes.clear();
}

void SceneManager::swap(const std::string& tag1, const std::string& tag2)
{
    int index[2] = { -1 };
    for (int i=0; i < (int)scenes.size(); ++i)
    {
        auto* s = scenes[i];
        if (s->tag == tag1)
            index[0] = i;
        else if (s->tag == tag2)
            index[1] = i;
    }

    if (index[0] < 0 or index[1] < 0)
        return;

    swap(index[0], index[1]);
}

void SceneManager::swap(std::size_t index1, std::size_t index2)
{
    std::size_t length = scenes.size();
    if (index1 >= length or index2 >= length)
        return;

    std::swap(scenes[index1], scenes[index2]);
}

void SceneManager::switch_to(const std::string& tag)
{
    auto it = std::find_if(scenes.begin(), scenes.end(), [&](Scene* s){ return s->tag == tag; });
    if (it == scenes.end())
        return;

// remove and stack elements together
    auto* scene = *it;
    scenes.erase(it);
// puts the element at the end of the queue
    scenes.push_back(scene);
}

void SceneManager::switch_to(std::size_t index)
{
    if (index >= scenes.size())
        return;

    auto* scene = scenes[index];
    scenes.erase(scenes.begin() + index);
    scenes.push_back(scene);
}

void SceneManager::remove(Scene* scene)
{
    scenes.erase(std::remove(scenes.begin(), scenes.end(), scene));
}

void SceneManager::remove(const std::string& tag)
{
    scenes.erase(std::remove_if(scenes.begin(), scenes.end(), [&](Scene* scene) { return scene->tag == tag; })); 
}

void SceneManager::remove(std::size_t index)
{
    if (index >= scenes.size())
        return;
    remove(scenes[index]);
}

void SceneManager::add(Scene* scene)
{
    scenes.push_back(scene);
}

void SceneManager::draw()
{
    if (scenes.empty())
        return;

    scenes[scenes.size()-1]->draw();
}

bool SceneManager::update()
{
    if (scenes.empty())
        return false;

    auto& scene = scenes[scenes.size()-1];
    if (!scene->update())
    {
        delete scene;
        scenes.pop_back();
    }
    
    return true;
}

Scene::Scene(const std::string& _tag) : tag(_tag)
{}
bool Scene::update()
{
    // overload
    return false;
}
