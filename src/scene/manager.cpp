#include <algorithm>
#include <cassert>

#include "../application/application.h"
#include "../ecs/components.h"
#include "scene.h"

namespace entix {
namespace core {

using SceneRef = std::shared_ptr<Scene>;
using Couple = std::pair<Scene*, Scene*>;

void SceneManager::load(const std::string& fileName) {
    auto scene = Application::Get().getSerializer().deserialize(fileName);
    EventManager::Get()
        ->emit(Scene::Event::LOADED)
        .attach<ecs::component::Tag>(scene->tag);
}

Scene& SceneManager::getActive() { return *scenes[0]; }

void SceneManager::setActive(const std::string& tag) {
    auto prev = scenes[0];
    auto it = std::find_if(scenes.begin(), scenes.end(),
                           [&](Scene* s) { return s->tag == tag; });

    // there is no scene with such tag
    if (it == scenes.end()) return;

    // remove and stack elements together
    scenes.erase(it);

    auto scene = *it;

    // puts the element at ontop of the queue
    auto prevScene = scenes.front();
    scenes.push_front(scene);

    EventManager::Get()
        ->emit(Scene::Event::CHANGED)
        .attach<SceneChange>(prevScene->tag, scene->tag);
}

void SceneManager::setActive(std::size_t index) {
    auto prev = scenes[0];
    if (index >= scenes.size()) return;

    auto scene = scenes[index];
    scenes.erase(scenes.begin() + index);

    auto prevScene = scenes.front();
    scenes.push_front(scene);

    EventManager::Get()
        ->emit(Scene::Event::CHANGED)
        .attach<SceneChange>(prevScene->tag, scene->tag);
}

void SceneManager::remove(const std::string& tag) {
    auto it = std::find_if(scenes.begin(), scenes.end(),
                           [&](Scene* scene) { return scene->tag == tag; });
    delete *it;
    scenes.erase(it);
}

void SceneManager::remove(std::size_t index) {
    if (index >= scenes.size()) return;

    auto it = scenes.begin() + index;
    delete *it;
    scenes.erase(it);
}

void SceneManager::push(Scene* scene) {
    if (std::find(scenes.begin(), scenes.end(), scene) == scenes.end())
        scenes.push_back(scene);
}

void SceneManager::render() {
    if (!scenes.empty()) scenes[0]->render();
}

bool SceneManager::update() {
    static bool last(true);
    if (!last) next();
    if (scenes.empty()) return false;
    last = scenes[0]->update();
    return true;
}

void SceneManager::next() {
    if (scenes.empty()) return;
    auto scene = scenes[0];

    auto prevScene = scene->tag;
    delete scene;
    scenes.pop_front();

    if (!scenes.empty())
        EventManager::Get()
            ->emit(Scene::Event::CHANGED)
            .attach<SceneChange>(prevScene, scenes.front()->tag);
}

SceneManager::~SceneManager() {
    for (auto scene : scenes) delete scene;
    scenes.clear();
}

// static
std::shared_ptr<SceneManager> SceneManager::Get() { return createInstance(); }

}  // namespace core
}  // namespace entix
