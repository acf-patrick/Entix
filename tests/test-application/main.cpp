#include "application/hook.h"
#include "scripts.h"
#include "serializer.h"
#include "systems.h"

using namespace entix::core;
using namespace entix::ecs;

class CustomHook : public ApplicationHook {
    EventListner eventListener;

   public:
    void startup() override {
        auto& application = Application::Get();
        application.setSerializer<CustomSerializer>();

        auto systemManager = SystemManager::Get();
        systemManager->add<WorldSystem>();

        eventListener.listen(Input::Event::QUIT, [&]() { application.quit(); })
            .listen(Input::Event::MOUSE_BUTTON_UP, [&]() {
                auto& entity =
                    SceneManager::Get()->getActive().getEntities().create();
                entity.useTemplate("prefabs/mob.entt");

                auto mousePos = Input::getMousePosition();
                if (entity.has<Mob>()) {
                    auto& body = *entity.get<Mob>().body;
                    body.SetTransform({mousePos.x / MtoPX, mousePos.y / MtoPX},
                                      0.0f);

                    entix::Logger::info("Controller") << "entity position set";
                    entix::Logger::endline();
                }
            });
    }
};

int main(int argc, char* argv[]) {
    Application::setup<CustomHook>();
    return entix::main(argc, argv);
}