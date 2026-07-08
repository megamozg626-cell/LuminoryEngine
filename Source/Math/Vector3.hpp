#pragma once

#include <cmath>
#include <algorithm>

namespace Luminary {

struct Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3() = default;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    explicit Vector3(float value) : x(value), y(value), z(value) {}

    // Operators
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(float scalar) const {
        if (scalar != 0.0f) {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        return Vector3();
    }

    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    bool operator==(const Vector3& v) const {
        const float epsilon = 1e-6f;
        return std::fabs(x - v.x) < epsilon &&
               std::fabs(y - v.y) < epsilon &&
               std::fabs(z - v.z) < epsilon;
    }

    float Dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 Cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float LengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3 Normalize() const {
        float len = Length();
        if (len > 0.0f) {
            return Vector3(x / len, y / len, z / len);
        }
        return Vector3();
    }

    Vector3 Reflect(const Vector3& normal) const {
        return *this - normal * (2.0f * Dot(normal));
    }

    Vector3 Lerp(const Vector3& to, float t) const {
        t = std::clamp(t, 0.0f, 1.0f);
        return *this + (to - *this) * t;
    }

    static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
};

} // namespace Luminary
