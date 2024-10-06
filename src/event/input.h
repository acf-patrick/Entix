/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Definition of input type injected when event occures
 */

#pragma once

#include <SDL.h>

#include <map>

#include "../util/geometry/vector.h"

namespace entix {
namespace core {

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

    static Mouse _mouse;

    static std::map<SDL_Scancode, bool> _keys;

   public:
    struct Event {
#define PROP(property) static const std::string property;
        PROP(QUIT)
        PROP(KEY_UP)
        PROP(KEY_DOWN)
        PROP(MOUSE_MOTION)
        PROP(MOUSE_WHEEL)
        PROP(MOUSE_BUTTON_UP)
        PROP(MOUSE_BUTTON_DOWN)
    };

    static bool isMousePressed(const std::string&);

    static VectorI getMousePosition();

    static bool isKeyPressed(SDL_Scancode);

    friend class EventManager;
};

}  // namespace core
}  // namespace entix
