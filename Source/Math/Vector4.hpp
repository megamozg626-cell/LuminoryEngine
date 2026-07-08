#pragma once

#include "Vector3.hpp"
#include <cmath>

namespace Luminary {

struct Vector4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    Vector4() = default;
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {}

    Vector3 ToVector3() const {
        if (w != 0.0f) {
            return Vector3(x / w, y / w, z / w);
        }
        return Vector3(x, y, z);
    }

    Vector4 operator+(const Vector4& v) const {
        return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    Vector4 operator-(const Vector4& v) const {
        return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    float Dot(const Vector4& v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Vector4 Normalize() const {
        float len = Length();
        if (len > 0.0f) {
            return Vector4(x / len, y / len, z / len, w / len);
        }
        return Vector4();
    }

    static Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
};

} // namespace Luminary
