/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Contains classes for Scene management
 */

#pragma once

#include <deque>
#include <memory>
#include <string>

#include "../ecs/group/group.h"
#include "../event/event.h"
#include "../manager/manager.h"

namespace entix {

namespace ecs {
class Entity;
}

namespace core {

class Application;
class Serializer;

struct SceneChange {
    std::string from;
    std::string to;

    SceneChange(const std::string& from, const std::string& to)
        : from(from), to(to) {}
};

struct SceneLoad {
    std::string name;

    SceneLoad(const std::string& sceneName) : name(sceneName) {}
};

// Scene Interface
class Scene {
   public:
    struct Event {
        static const std::string LOADED;
        static const std::string CHANGED;
    };

    // Save to file
    void save(const std::string& fileName = "");

    // Merge given scene into this scene
    void merge(const Scene&);

    // Set this scene to be active
    void setActive();

    ecs::Entity& createEntity();

    ecs::Group& getEntities();

    ecs::Entity* getEntity(const std::string& tag);

    static const std::string FOLDER;
    static const std::string FILE_EXTENSION;

   protected:
    Scene(const std::string&);
    virtual ~Scene() = default;

    // pop the scene from manager if this method return false
    virtual bool update();

    virtual void render();

    std::string tag = "Scene";

    // manages entities
    ecs::Group _entities;

    std::vector<std::string> _freeSystems;
    std::vector<std::vector<std::string>> _systemGroups;

    bool _systemsActivated = false;

    bool active = true;

    friend class Serializer;
    friend class SceneManager;
};

class EmptyScene : public Scene {
   public:
    EmptyScene() = default;
};

class SceneManager : Manager<SceneManager> {
   public:
    // Load a scene
    std::shared_ptr<Scene> load(const std::string& sceneName);

    // Return active scene
    Scene& getActive();

    bool activateOrLoad(const std::string& sceneName);

    // Set scene at index to be active
    bool setActive(size_t);

    // Set scene with the given name to be active
    bool setActive(const std::string&);

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

    void setListOfScenes(const std::vector<std::string>& sceneNames);

    static std::shared_ptr<SceneManager> Get();

   private:
    std::shared_ptr<Scene> _currentScene;
    std::deque<std::string> _sceneNames;

    SceneManager() = default;
    ~SceneManager() = default;

    bool loadCurrentScene();

    friend class Application;
    friend class Manager<SceneManager>;
    friend class Scene;
};

}  // namespace core
}  // namespace entix
