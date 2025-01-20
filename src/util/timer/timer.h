#pragma once

#include <SDL2/SDL.h>

#include <optional>

namespace entix {
namespace util {

class Timer {
    Uint32 _startTick;
    Uint32 _initialDelay, _delay;
    bool _running = true;

   public:
    Timer(Uint32 delay)
        : _startTick(SDL_GetTicks()), _initialDelay(delay), _delay(delay) {}

    Uint32 getElapsedTime() const { return SDL_GetTicks() - _startTick; }

    Uint32 getRemainingTime() const {
        const auto elapsedTime = getElapsedTime();
        if (elapsedTime > _delay) return 0;

        return _delay - elapsedTime;
    }

    /**
     * returns progress as a number between 0 and 1
     */
    float getProgress() const {
        if (_delay == 0.0) return 0.0;
        return getElapsedTime() / (float)_delay;
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

// Does not start the timer immediatly after instanciation
class LazyTimer {
    std::optional<Timer> _timer;
    Uint32 _delay;

   public:
    LazyTimer(Uint32 delay) : _delay(delay) {}

    Uint32 getElapsedTime() const {
        if (_timer) return _timer->getElapsedTime();
        return 0;
    }

    Uint32 getRemainingTime() const {
        if (_timer) return _timer->getRemainingTime();
        return _delay;
    }

    /**
     * returns progress as a number between 0 and 1
     */
    float getProgress() const {
        if (_timer) return _timer->getProgress();
        return 0.0;
    }

    bool isDone() const {
        if (_timer) return _timer->isDone();
        return false;
    }

    bool isRunning() const {
        if (_timer) return _timer->isRunning();
        return false;
    }

    bool isPaused() const {
        if (_timer) return _timer->isPaused();
        return true;
    }

    void pause() {
        if (_timer) _timer->pause();
    }

    void resume() {
        if (_timer)
            _timer->resume();
        else
            _timer = Timer(_delay);
    }

    void reset() {
        if (_timer) _timer->reset();
    }

    void reset(Uint32 delay) {
        if (_timer)
            _timer->reset(delay);
        else {
            _timer = Timer(delay);
            _delay = delay;
        }
    }
};

}  // namespace util
}  // namespace entix