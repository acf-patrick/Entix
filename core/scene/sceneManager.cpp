#include <cassert>
#include <algorithm>

#include "scene.h"
#include "../application/application.h"
#include "../ecs/components.h"

using SceneRef = std::shared_ptr<Scene>;

void SceneManagerType::load(const std::string& fileName)
{
    auto scene = Application::serializer->deserialize(fileName);
    EventManager->emit(Input.SCENE_LOADED).attach<Component::tag>(scene->tag);
}

Scene& SceneManagerType::getActive()
{
    return *scenes[0];
}

void SceneManagerType::setActive(const std::string& tag)
{
    auto it = std::find_if(scenes.begin(), scenes.end(), [&](Scene* s){ return s->tag == tag; });

    // there is no scene with such tag
    if (it == scenes.end())
        return;

    // remove and stack elements together
    scenes.erase(it);

    auto scene = *it;
    // puts the element at the end of the queue
    scenes.push_front(scene);
    EventManager->emit(Input.SCENE_CHANGED).attach<Component::tag>(scene->tag);
}

void SceneManagerType::setActive(std::size_t index)
{
    if (index >= scenes.size())
        return;

    auto scene = scenes[index];
    scenes.erase(scenes.begin() + index);

    scenes.push_front(scene);
    EventManager->emit(Input.SCENE_CHANGED).attach<Component::tag>(scene->tag);;
}

void SceneManagerType::remove(const std::string& tag)
{   
    auto it = std::find_if(scenes.begin(), scenes.end(), [&](Scene* scene) { return scene->tag == tag; });
    delete *it;
    scenes.erase(it);
}

void SceneManagerType::remove(std::size_t index)
{
    if (index >= scenes.size())
        return;

    auto it = scenes.begin() + index;
    delete *it;
    scenes.erase(it);
}

void SceneManagerType::push(Scene* scene)
{
    if (std::find(scenes.begin(), scenes.end(), scene) == scenes.end())
        scenes.push_back(scene);
}

void SceneManagerType::render()
{
    if (!scenes.empty())
        scenes[0]->render();
}

bool SceneManagerType::update()
{
    static bool last(true);
    if (!last)
        next();
    if (scenes.empty())
        return false;
    last = scenes[0]->update();
    return true;
}

void SceneManagerType::next()
{
    if (scenes.empty())
        return;
    auto scene = scenes[0];
    delete scene;
    scenes.pop_front();
    EventManager->emit(Input.SCENE_CHANGED).attach<Component::tag>(scene->tag);;
}

SceneManagerType::~SceneManagerType()
{
    for (auto scene : scenes)
        delete scene;
    scenes.clear();
}
