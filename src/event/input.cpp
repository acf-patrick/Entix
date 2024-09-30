#include "input.h"

#define EVENT_PROP(property) const std::string Input::Event::property

EVENT_PROP(QUIT) = "SDL quit";
EVENT_PROP(KEY_DOWN) = "SDL key down";
EVENT_PROP(KEY_UP) = "SDL key up";
EVENT_PROP(MOUSE_MOTION) = "SDL mouse motion";
EVENT_PROP(MOUSE_WHEEL) = "SDL mouse wheel";
EVENT_PROP(MOUSE_BUTTON_UP) = "SDL mouse button up";
EVENT_PROP(MOUSE_BUTTON_DOWN) = "SDL mouse button down";

Input::Mouse Input::_mouse;
std::map<SDL_Scancode, bool> Input::_keys;

bool Input::isKeyPressed(SDL_Scancode scanCode) {
    auto it = _keys.find(scanCode);
    if (it == _keys.end()) return false;

    auto& [_, pressed] = *it;
    return pressed;
}

bool Input::isMousePressed(const std::string& button) {
    return _mouse.isPressed(button);
}

VectorI Input::getMousePosition() { return _mouse.getPosition(); }
