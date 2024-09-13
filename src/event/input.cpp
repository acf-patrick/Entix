#include "input.h"

Input* Input::instance = nullptr;

Input::Input() { instance = this; }

bool Input::isKeyPressed(SDL_Scancode scanCode) const {
    auto it = _keys.find(scanCode);
    if (it == _keys.end()) return false;

    auto& [_, pressed] = *it;
    return pressed;
}

bool Input::isMousePressed(const std::string& button) const {
    return _mouse.isPressed(button);
}

VectorI Input::getMousePosition() const { return _mouse.getPosition(); }

// static
Input& Input::Get() {
    if (!instance) instance = new Input;
    return *instance;
}