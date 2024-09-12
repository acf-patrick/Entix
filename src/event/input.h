/**
 * @author acf-patrick (miharisoap@gmail.com)
 * 
 * Definition of input type injected when event occures
 */

#pragma once

#include "../util/geometry/vector.h"

#include <map>
#include <SDL.h>

struct InputType 
{
    const std::string 
        QUIT = "SDL quit",
        KEY_UP = "SDL key up",
        KEY_DOWN = "SDL key down",
        MOUSE_WHEEL = "SDL mouse wheel",
        MOUSE_MOTION = "SDL mouse motion",
        MOUSE_BUTTON_UP = "SDL mouse button up",
        MOUSE_BUTTON_DOWN = "SDL mouse button down",
        SCENE_LOADED = "Scene loaded",
        SCENE_CHANGED = "Scene changed";

    struct Mouse
    {
        bool isPressed(const std::string& button)
        {
            std::map<std::string, int> bind = {
                { "left", SDL_BUTTON_LMASK },
                { "right", SDL_BUTTON_RMASK },
                { "middle", SDL_BUTTON_MMASK }
            };
            return bind[button] & SDL_GetMouseState(NULL, NULL);
        }

        VectorI getPosition()
        {
            VectorI ret;
            SDL_GetMouseState(&ret.x, &ret.y);
            return ret;
        }
    };
    Mouse mouse;

    std::map<SDL_Scancode, bool> keys;
};

extern InputType Input;