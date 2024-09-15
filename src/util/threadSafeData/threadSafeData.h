/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Wrapper to ensure data manipulation safety across threads
 */

#pragma once

#include <memory>
#include <mutex>

template <typename T>
class TheadSafeData {
    std::shared_ptr<T> _data;
};