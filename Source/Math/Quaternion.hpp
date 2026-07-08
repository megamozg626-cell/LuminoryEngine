#pragma once

#include "Vector3.hpp"
#include <cmath>

namespace Luminary {

class Quaternion {
public:
    float x, y, z, w;

    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Vector3 operator*(const Vector3& v) const {
        Vector3 u(x, y, z);
        Vector3 uv = u.Cross(v);
        Vector3 uuv = u.Cross(uv);
        return v + (uv * (2.0f * w)) + (uuv * 2.0f);
    }

    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    static Quaternion Identity() {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Quaternion FromEulerAngles(float pitch, float yaw, float roll) {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        return Quaternion(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion result;
        float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        Quaternion bb = b;

        if (dot < 0.0f) {
            dot = -dot;
            bb.x = -bb.x;
            bb.y = -bb.y;
            bb.z = -bb.z;
            bb.w = -bb.w;
        }

        if (dot < 0.9995f) {
            float theta_0 = std::acos(dot);
            float theta = theta_0 * t;
            Quaternion q2 = (bb - (a * dot)).Normalize();
            result = (a * std::cos(theta)) + (q2 * std::sin(theta));
        } else {
            result = Quaternion(
                Math::Lerp(a.x, bb.x, t),
                Math::Lerp(a.y, bb.y, t),
                Math::Lerp(a.z, bb.z, t),
                Math::Lerp(a.w, bb.w, t)
            );
        }
        return result.Normalize();
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Quaternion Normalize() const {
        float len = Length();
        if (len > 0.0f) {
            return Quaternion(x / len, y / len, z / len, w / len);
        }
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }
};

} // namespace Luminary
