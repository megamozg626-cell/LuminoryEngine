#pragma once

#include <cmath>
#include "Math.hpp"

namespace Luminary {

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator*(const Vector3& other) const {
        return Vector3(x * other.x, y * other.y, z * other.z);
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
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    float Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    Vector3 Lerp(const Vector3& other, float t) const {
        return Vector3(
            Math::Lerp(x, other.x, t),
            Math::Lerp(y, other.y, t),
            Math::Lerp(z, other.z, t)
        );
    }

    static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, -1.0f); }
    static Vector3 Back() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
    static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
};

} // namespace Luminary
