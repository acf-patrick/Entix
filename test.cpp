#include <iostream>
#include <application/application.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class App : public Application
{
public:
    App() : Application("test", 800, 600)
    {}

    void run()
    {
    	SDL_Point wSize, tSize;
    	SDL_Rect src, dest;
    	SDL_Texture* texture;

    	texture = IMG_LoadTexture(_renderer, "texture.jpg");
    	SDL_GetWindowSize(_window, &wSize.x, &wSize.y);
		SDL_QueryTexture(texture, NULL, NULL, &tSize.x, &tSize.y);

		dest = {
			int(0.25*(wSize.x)), int(0.25*(wSize.y)),
			int(0.5*wSize.x), int(0.5*wSize.y)
		};

    	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    	SDL_RenderClear(_renderer);
    	SDL_RenderCopyEx(_renderer, texture, NULL, &dest, 45, NULL, SDL_FLIP_NONE);

    	SDL_RenderPresent(_renderer);
		SDL_Delay(3000);

		SDL_DestroyTexture(texture);
    }

};

Application* createApp()
{
    return new App();
}
