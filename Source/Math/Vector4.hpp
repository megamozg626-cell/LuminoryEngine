#pragma once

namespace Luminary {

class Vector4 {
public:
    float x, y, z, w;

    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    static Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
    static Vector4 One() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
};

} // namespace Luminary
