#pragma once

#include <map>

struct InputType 
{
    const std::string 
        QUIT = "SDL quit",
        KEY_UP = "SDL key up",
        KEY_DOWN = "SDL key down",
        MOUSE_MOTION = "SDL mouse motion",
        MOUSE_BUTTON_UP = "SDL mouse button up",
        MOUSE_BUTTON_DOWN = "SDL mouse button down";

    struct Mouse
    {
        bool pressed;
        int x, y;
    };
    Mouse mouse;

    std::map<SDL_Scancode, bool> keys;
};

extern InputType Input;