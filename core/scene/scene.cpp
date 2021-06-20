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
    for (int i=0; (index[0] < 0 or index[1] < 0) and i < (int)scenes.size(); ++i)
    {
        auto s = scenes[i];
        if (s->tag == tag1)
            index[0] = i;
        else if (s->tag == tag2)
            index[1] = i;
    }

    if (index[0] >= 0 and index[1] >= 0)
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

// there is no scene with such tag
    if (it == scenes.end())
        return;

// remove and stack elements together
    auto* scene = *it;
    scenes.erase(it);

// puts the element at the end of the queue
    scenes.push_front(scene);
}

void SceneManager::switch_to(std::size_t index)
{
    if (index >= scenes.size())
        return;

    auto scene = scenes[index];
    scenes.erase(scenes.begin() + index);

    scenes.push_front(scene);
}

void SceneManager::remove(const std::string& tag)
{
    auto it = std::find_if(scenes.begin(), scenes.end(), [&](Scene* scene) { return scene->tag == tag; });
    delete *it;
    scenes.erase(it);
}

void SceneManager::remove(std::size_t index)
{
    if (index >= scenes.size())
        return;

    auto it = scenes.begin() + index;
    delete *it;
    scenes.erase(it);
}

void SceneManager::push(Scene* scene)
{
    if (std::find(scenes.begin(), scenes.end(), scene) == scenes.end())
        scenes.push_back(scene);
}

bool SceneManager::update()
{
    static Scene* toRemove = nullptr;
    if (toRemove)
    {
        delete toRemove;
        toRemove = nullptr;
    }
    if (scenes.empty())
        return false;

    auto& scene = scenes[0];
    if (!scene->update())
    {
        scenes.pop_front();
        toRemove = scene;
    }
    
    return true;
}

void SceneManager::next()
{
    if (scenes.empty())
        return;

    auto& scene = scenes[0];
    scenes.pop_front();
    delete scene;
}

Scene::Scene(const std::string& _tag) : tag(_tag)
{}

bool Scene::update()
{
    entities.for_each([](Entity& entity) { entity.Update(); });
    entities.for_each([](Entity& entity) { entity.Render(); });
    return true;
}
