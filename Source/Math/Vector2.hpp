#pragma once

#include <cmath>
#include <algorithm>

namespace Luminary {

struct Vector2 {
    float x = 0.0f;
    float y = 0.0f;

    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}

    // Operators
    Vector2 operator+(const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const {
        if (scalar != 0.0f) {
            return Vector2(x / scalar, y / scalar);
        }
        return Vector2();
    }

    Vector2& operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    float Dot(const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    float Cross(const Vector2& v) const {
        return x * v.y - y * v.x;
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
        return Vector2();
    }

    Vector2 Reflect(const Vector2& normal) const {
        return *this - normal * (2.0f * Dot(normal));
    }

    static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
    static Vector2 One() { return Vector2(1.0f, 1.0f); }
    static Vector2 Right() { return Vector2(1.0f, 0.0f); }
    static Vector2 Up() { return Vector2(0.0f, 1.0f); }
};

} // namespace Luminary
