#pragma once

#include <cmath>
#include "Math.hpp"

namespace Luminary {

class Vector2 {
public:
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    float Length() const {
        return std::sqrt(x * x + y * y);
    }

    float LengthSquared() const {
        return x * x + y * y;
    }

    Vector2 Normalize() const {
        float len = Length();
        if (len > 0.0f) {
            return Vector2(x / len, y / len);
        }
        return Vector2(0.0f, 0.0f);
    }

    float Dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    Vector2 Lerp(const Vector2& other, float t) const {
        return Vector2(
            Math::Lerp(x, other.x, t),
            Math::Lerp(y, other.y, t)
        );
    }

    static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
    static Vector2 One() { return Vector2(1.0f, 1.0f); }
    static Vector2 Right() { return Vector2(1.0f, 0.0f); }
    static Vector2 Up() { return Vector2(0.0f, 1.0f); }
};

} // namespace Luminary
