#include <iostream>
#include <application/application.h>
#include <SDL2/SDL.h>

class App : public Application
{
public:
    App() : Application("test", 640, 480)
    {}

    void run()
    {
    	SDL_Delay(3000);
    }

};

Application* createApp()
{
    return new App();
}
