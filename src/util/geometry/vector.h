/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Definition of a 2D vector class
 */

#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

namespace entix {

template <typename T>
struct Vector {
    Vector() : x(0), y(0) {}

    Vector(T _x, T _y) : x(_x), y(_y) {}

    void set(T _x, T _y) {
        x = _x;
        y = _y;
    }

    void normalize() {
        auto& self = *this;
        self /= magnitude();
    }

    double magnitude() const { return std::hypot(x, y); }

    // Vector operations

    Vector<T> operator-() const { return {-x, -y}; }

    template <typename _T>
    Vector<T> operator+(const Vector<_T>& rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    template <typename _T>
    Vector<T>& operator+=(const Vector<_T>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template <typename _T>
    Vector<T> operator-(const Vector<_T>& other) const {
        return *this + (-other);
    }

    Vector<T>& operator-=(const Vector<T>& other) {
        *this += (-other);
        return *this;
    }

    // Scalar and vectors operations

    template <typename _T>
    Vector<_T> operator*(_T scalar) const {
        return {scalar * x, scalar * y};
    }

    template <typename _T>
    Vector<T>& operator*=(_T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    template <typename _T>
    Vector<_T> operator/(_T scalar) const {
        assert(scalar != 0 && "Arithmetic Error : Division by zero!");
        return {x / scalar, y / scalar};
    }

    template <typename _T>
    Vector<T>& operator/=(_T scalar) {
        assert(scalar != 0 && "Arithmetic Error : Division by zero!");
        x /= scalar;
        y /= scalar;
        return *this;
    }

    operator bool() const { return x != 0 or y != 0; }

    // useful cast
    operator Vector<int>() const { return Vector<int>(x, y); }

    operator Vector<float>() const { return Vector<float>(x, y); }

    operator Vector<double>() const { return Vector<double>(x, y); }

    T x = 0;
    T y = 0;
};

using VectorI = Vector<int>;
using VectorF = Vector<float>;
using VectorD = Vector<double>;

}  // namespace entix

template <typename T>
std::ostream& operator<<(std::ostream& flux, const entix::Vector<T>& vector) {
    flux << '(' << vector.x << ", " << vector.y << ')';
    return flux;
}

// dot product
template <typename T1, typename T2>
double operator*(const entix::Vector<T1>& v1, const entix::Vector<T2>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
