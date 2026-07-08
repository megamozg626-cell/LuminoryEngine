#pragma once

#include "Vector3.hpp"
#include "Vector4.hpp"
#include <cmath>

namespace Luminary {

struct Quaternion {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    Quaternion() = default;
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    static Quaternion Identity() {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Quaternion FromAxisAngle(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float sinHalf = std::sin(halfAngle);
        Vector3 normalizedAxis = axis.Normalize();
        return Quaternion(
            normalizedAxis.x * sinHalf,
            normalizedAxis.y * sinHalf,
            normalizedAxis.z * sinHalf,
            std::cos(halfAngle)
        );
    }

    static Quaternion FromEulerAngles(float roll, float pitch, float yaw) {
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

    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }

    Vector3 operator*(const Vector3& v) const {
        Vector3 u(x, y, z);
        Vector3 result = u * (2.0f * u.Dot(v)) + v * (w * w - u.Dot(u)) + u.Cross(v) * (2.0f * w);
        return result;
    }

    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Quaternion Normalize() const {
        float len = Length();
        if (len > 0.0f) {
            return Quaternion(x / len, y / len, z / len, w / len);
        }
        return Identity();
    }

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion q1 = a.Normalize();
        Quaternion q2 = b.Normalize();

        float dotProduct = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
        if (dotProduct < 0.0f) {
            q2 = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
            dotProduct = -dotProduct;
        }

        dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
        float theta = std::acos(dotProduct);
        float sinTheta = std::sin(theta);

        if (sinTheta < 0.001f) {
            return Quaternion(
                q1.x + t * (q2.x - q1.x),
                q1.y + t * (q2.y - q1.y),
                q1.z + t * (q2.z - q1.z),
                q1.w + t * (q2.w - q1.w)
            ).Normalize();
        }

        float w1 = std::sin((1.0f - t) * theta) / sinTheta;
        float w2 = std::sin(t * theta) / sinTheta;

        return Quaternion(
            w1 * q1.x + w2 * q2.x,
            w1 * q1.y + w2 * q2.y,
            w1 * q1.z + w2 * q2.z,
            w1 * q1.w + w2 * q2.w
        );
    }
};

} // namespace Luminary
