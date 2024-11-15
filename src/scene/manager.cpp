#include <algorithm>
#include <cassert>

#include "../application/application.h"
#include "../ecs/components.h"
#include "scene.h"

namespace entix {
namespace core {

std::shared_ptr<Scene> SceneManager::load(const std::string& sceneName) {
    if (auto scene = Application::Get().getSerializer().deserialize(sceneName);
        scene) {
        EventManager::Get()
            ->emit(Scene::Event::LOADED)
            .attach<SceneLoad>(scene->tag);
        return std::shared_ptr<Scene>(scene,
                                      [](Scene* scene) { delete scene; });
    }
    return nullptr;
}

Scene& SceneManager::getActive() { return *_currentScene; }

bool SceneManager::activateOrLoad(const std::string& tag) {
    if (!setActive(tag)) {
        Logger::info("SceneManager") << "Loading " << tag;
        Logger::endline();

        _sceneNames.push_front(tag);

        if (!loadCurrentScene()) {
            Logger::error("SceneManager") << "Unable to load" << tag;
            Logger::endline();

            _sceneNames.pop_front();
            return false;
        }
    }

    return true;
}

bool SceneManager::setActive(const std::string& tag) {
    const auto& prevSceneName = _sceneNames[0];
    if (tag == prevSceneName) {
        Logger::warn("SceneManager") << "Scene '" << tag << "' already active";
        Logger::endline();

        return true;
    }

    auto it = std::find_if(
        _sceneNames.begin(), _sceneNames.end(),
        [&](const std::string& sceneName) { return sceneName == tag; });

    // there is no scene with such tag
    if (it == _sceneNames.end()) {
        Logger::warn("SceneManager")
            << "Unable to find scene '" << tag << "' in the stack";
        Logger::endline();

        return false;
    }

    auto sceneName = *it;
    _sceneNames.erase(it);
    _sceneNames.push_front(sceneName);

    EventManager::Get()
        ->emit(Scene::Event::CHANGED)
        .attach<SceneChange>(prevSceneName, tag);

    return true;
}

bool SceneManager::setActive(size_t index) {
    if (index == 0) {
        Logger::warn("SceneManager")
            << "Scene ontop of the stack is always active";
        Logger::endline();

        return true;
    }

    if (index >= _sceneNames.size()) return false;

    auto prevScene = _sceneNames.front();

    auto scene = _sceneNames[index];
    _sceneNames.erase(_sceneNames.begin() + index);
    _sceneNames.push_front(scene);

    loadCurrentScene();

    EventManager::Get()
        ->emit(Scene::Event::CHANGED)
        .attach<SceneChange>(prevScene, scene);

    return true;
}

void SceneManager::remove(const std::string& tag) {
    auto removingCurrentScene = tag == _sceneNames.front();
    if (tag == _sceneNames.front()) {
        _sceneNames.pop_front();
        loadCurrentScene();
    } else
        _sceneNames.erase(
            std::remove(_sceneNames.begin(), _sceneNames.end(), tag));
}

void SceneManager::remove(std::size_t index) {
    if (index >= _sceneNames.size()) return;

    if (index == 0) {
        _sceneNames.pop_front();
        loadCurrentScene();
    } else {
        auto it = _sceneNames.begin() + index;
        _sceneNames.erase(it);
    }
}

void SceneManager::setListOfScenes(const std::vector<std::string>& sceneNames) {
    for (auto& sceneName : sceneNames) _sceneNames.push_back(sceneName);
    loadCurrentScene();
}

void SceneManager::render() {
    if (_currentScene) _currentScene->render();
}

bool SceneManager::update() {
    static bool lastUpdateForCurrentScene(true);
    if (!lastUpdateForCurrentScene) next();

    if (_sceneNames.empty()) return false;

    lastUpdateForCurrentScene = _currentScene->update();

    return true;
}

void SceneManager::next() {
    if (_sceneNames.empty()) return;
    auto prevScene = _sceneNames[0];

    _sceneNames.pop_front();

    if (!_sceneNames.empty()) {
        loadCurrentScene();
        EventManager::Get()
            ->emit(Scene::Event::CHANGED)
            .attach<SceneChange>(prevScene, _sceneNames.front());
    }
}

bool SceneManager::loadCurrentScene() {
    if (_sceneNames.size() > 0) {
        if (auto scene = load(_sceneNames.front()); scene) {
            _currentScene = scene;
            return true;
        }
    }
    return false;
}

// static
std::shared_ptr<SceneManager> SceneManager::Get() { return createInstance(); }

}  // namespace core
}  // namespace entix
