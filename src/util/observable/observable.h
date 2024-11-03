/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Inspired by Observable in common state management
 */

#pragma once

#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <utility>

namespace entix {
namespace util {

/**
 * The Observable class
 *
 * Ensure immutability and notify the system of data changes
 *
 * using T = std::vector<int>;
 * Observable<T> array$;
 *
 * // attach a call back to the observable
 * array$.submit([](T& array) {
 * 	std::cout << "Changes have been made!" << std::endl;
 * });
 *
 * // modifying
 * array$.next([](T& array) {
 * 	array.push_back(5);
 * });
 *
 * array$.next(T());
 *
 * Easy! ^^
 */
template <typename T>
class Observable {
   protected:
    T *_raw;
    std::map<std::string, std::function<void(T &)>> _callbacks;
    int _lastKey = 0;

    void callCallbacks() {
        for (auto &[_, callback] : _callbacks) callback(*_raw);
    }

   public:
    /**
     * @param params Values used to initialized the data held by the Observable
     */
    template <typename... TArgs>
    Observable(TArgs &&...params) {
        _raw = new T(std::forward<TArgs>(params)...);
    }

    Observable() { _raw = new T; }

    virtual ~Observable() { delete _raw; }

    /**
     * @return Current value
     */
    T value() { return *_raw; }

    /**
     * Change value of the Observable, and
     * call all callback listners in the same time
     */
    void next(T value) {
        *_raw = value;
        callCallbacks();
    }

    /**
     * Change value of the Observable, and
     * call all callback listners in the same time
     *
     * @param operation Perform operation on Observable's current value
     */
    void next(const std::function<void(T &)> &operation) {
        operation(*_raw);
        callCallbacks();
    }

    /**
     * @param callback Change made on this Observable will trigger the callback
     */
    virtual void submit(const std::function<void(T &)> &callback) {
        std::stringstream ss;
        ss << _lastKey;
        _callbacks[ss.str()] = callback;
        _lastKey++;
    }

    /**
     * @param key Associated key to the callback. Used to indentify the function
     * when removal is needed
     * @param callback Change made on this Observable will trigger the callback
     */
    virtual void submit(const std::string &key,
                        const std::function<void(T &)> &callback) {
        _callbacks[key] = callback;
    }

    /**
     * Remove the associated callback from listners
     */
    void distach(const std::string &key) { _callbacks.erase(key); }

    /**
     * Remove all callbacks
     */
    void distachAll() { _callbacks.clear(); }
};

/**
 * Custom Observable
 *
 * The difference is that callback will always be called at least once,
 * while for a mere Observable, only change will trigger callbacks.
 *
 * @see Observable
 */
template <typename T>
class Subject : public Observable<T> {
   public:
    template <typename... TArgs>
    Subject(TArgs &&...args) : Observable<T>(std::forward<TArgs>(args)...) {}

    Subject() : Observable<T>() {}

    void submit(const std::function<void(T &)> &callback) override {
        Observable<T>::submit(callback);
        callback(*this->_raw);
    }

    void submit(const std::string &key,
                const std::function<void(T &)> &callback) override {
        Observable<T>::submit(key, callback);
        callback(*this->_raw);
    }
};

}  // namespace util
}  // namespace entix
