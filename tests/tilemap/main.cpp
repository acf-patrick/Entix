#include <core.h>

class CustomHook : public entix::core::ApplicationHook {
    entix::core::EventListner eventListener;

   public:
    void startup() override {
        eventListener
            .listen(entix::core::Input::Event::QUIT,
                    [&] { entix::core::Application::Quit(); })
            .listen(entix::core::Input::Event::KEY_DOWN, [] {
                const float speed = 2;
                auto& scene = entix::core::SceneManager::Get()->getActive();
                if (auto camera = scene.getEntity("main camera"); camera) {
                    auto& cameraPosition =
                        camera->get<entix::ecs::component::Transform>()
                            .position;

                    if (entix::core::Input::isKeyPressed(SDL_SCANCODE_UP))
                        cameraPosition.y -= speed;
                    if (entix::core::Input::isKeyPressed(SDL_SCANCODE_DOWN))
                        cameraPosition.y += speed;
                    if (entix::core::Input::isKeyPressed(SDL_SCANCODE_LEFT))
                        cameraPosition.x -= speed;
                    if (entix::core::Input::isKeyPressed(SDL_SCANCODE_RIGHT))
                        cameraPosition.x += speed;
                }
            });
    }
};

int main(int argc, char* argv[]) {
    entix::core::Application::setup<CustomHook>();
    return entix::main(argc, argv);
}