#pragma once

#include <ostream>
#include <cassert>
#include <cmath>

template <typename T>
struct Vector
{
    Vector() = default;

    Vector(T _x, T _y) : x(_x), y(_y)
    {}

    void normalize()
    {
        auto& self = *this;
        self /= magnitude();
    }

    double magnitude() const
    { return std::hypot(x, y); }

// Vector operations

    Vector<T>& operator- () const
    {
        x *= -1;
        y *= -1;
        return *this;
    }

    template <typename _T>
    Vector<double> operator+ (const Vector<_T>& other) const
    { return { x+other.x, y+other.y }; }

    template <typename _T>
    Vector<T>& operator+= (const Vector<_T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template <typename _T>
    Vector<double> operator- (const Vector<_T>& other) const
    { return *this + (-other); }

    Vector<T>& operator-= (const Vector<T>& other)
    {
        *this += (-other);
        return *this;
    }

    /* Dot product */
    template <typename _T>
    double operator* (const Vector<_T>& other) const
    { return x*other.x + y*other.y; }

// Scalar and vectors operations

    template <typename TScalar>
    Vector<double> operator* (TScalar scalar) const
    { return { scalar * x, scalar * y }; }

    template <typename TScalar>
    Vector<T>& operator*= (TScalar scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    template <typename TScalar>
    Vector<double> operator/ (TScalar scalar) const
    {
        assert(scalar != 0 && "Arithmetic Error : Division by zero!");

        auto& self = *this;
        return self*(1/double(scalar));
    }

    template <typename TScalar>
    Vector<T>& operator/= (TScalar scalar)
    {
        assert(scalar != 0 && "Arithmetic Error : Division by zero!");

        auto& self = *this;
        return self *= (1/double(scalar));
    }

    operator Vector<int> () const
    { return Vector<int>(x, y); }
    
    operator Vector<float> () const
    { return Vector<float>(x, y); }

    operator Vector<double> () const
    { return Vector<double>(x, y); }

    T x = 0;
    T y = 0;
};

using VectorI = Vector<int>;
using VectorF = Vector<float>;

template <typename T>
std::ostream& operator<<(std::ostream& flux, const Vector<T>& vector)
{
    flux << '(' << vector.x << ", " << vector.y << ')';
    return flux;
}