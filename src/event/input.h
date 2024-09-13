/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Definition of input type injected when event occures
 */

#pragma once

#include <SDL.h>

#include <map>

#include "../util/geometry/vector.h"

class EventManager;

class Input {
    struct Mouse {
        bool isPressed(const std::string& button) const {
            std::map<std::string, int> bind = {{"left", SDL_BUTTON_LMASK},
                                               {"right", SDL_BUTTON_RMASK},
                                               {"middle", SDL_BUTTON_MMASK}};
            return bind[button] & SDL_GetMouseState(NULL, NULL);
        }

        VectorI getPosition() const {
            VectorI ret;
            SDL_GetMouseState(&ret.x, &ret.y);
            return ret;
        }
    };

    Mouse _mouse;

    std::map<SDL_Scancode, bool> _keys;

    static Input* instance;

    Input();

   public:
    const std::string QUIT = "SDL quit", KEY_UP = "SDL key up",
                      KEY_DOWN = "SDL key down",
                      MOUSE_WHEEL = "SDL mouse wheel",
                      MOUSE_MOTION = "SDL mouse motion",
                      MOUSE_BUTTON_UP = "SDL mouse button up",
                      MOUSE_BUTTON_DOWN = "SDL mouse button down",
                      SCENE_LOADED = "Scene loaded",
                      SCENE_CHANGED = "Scene changed";

    static Input& Get();

    bool isMousePressed(const std::string&) const;

    VectorI getMousePosition() const;

    bool isKeyPressed(SDL_Scancode) const;

    friend class EventManager;
};