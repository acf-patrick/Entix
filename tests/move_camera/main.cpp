#include <core.h>

#include "./serializer.h"

class CustomHook : public entix::core::ApplicationHook {
    entix::core::EventListner eventListner;

   public:
    void startup() override {
        eventListner.listen(entix::core::Input::Event::QUIT,
                            [] { entix::core::Application::Quit(); });

        auto& application = entix::core::Application::Get();
        application.setSerializer<Serializer>();
    }
};

int main(int argc, char* argv[]) {
    entix::core::Application::setup<CustomHook>();
    return entix::main(argc, argv);
}
