#pragma once

#include <SDL2/SDL.h>

namespace entix {
namespace util {

class Timer {
    Uint32 _startTick;
    Uint32 _initialDelay, _delay;
    bool _running;

   public:
    Timer(Uint32 delay, bool running = true)
        : _startTick(SDL_GetTicks()),
          _initialDelay(delay),
          _delay(delay),
          _running(running) {}

    Uint32 getElapsedTime() const { return SDL_GetTicks() - _startTick; }

    Uint32 getRemainingTime() const {
        const auto elapsedTime = getElapsedTime();
        if (elapsedTime > _delay) return 0;

        return _delay - elapsedTime;
    }

    bool isDone() const { return getRemainingTime() == 0; }

    bool isRunning() const { return _running; }

    bool isPaused() const { return !_running; }

    void pause() {
        _delay = getRemainingTime();
        _running = false;
    }

    void resume() {
        if (_running) return;

        _startTick = SDL_GetTicks();
        _running = true;
    }

    void reset() {
        _startTick = SDL_GetTicks();
        _delay = _initialDelay;
        _running = true;
    }

    void reset(Uint32 delay) {
        _startTick = SDL_GetTicks();
        _initialDelay = delay;
        _delay = delay;
        _running = true;
    }
};

}  // namespace util
}  // namespace entix