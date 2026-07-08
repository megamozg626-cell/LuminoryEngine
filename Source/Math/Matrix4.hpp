#pragma once

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include <cmath>
#include <cstring>

namespace Luminary {

struct Matrix4 {
    float m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    Matrix4() {
        Identity();
    }

    Matrix4(float m00, float m01, float m02, float m03,
             float m10, float m11, float m12, float m13,
             float m20, float m21, float m22, float m23,
             float m30, float m31, float m32, float m33) {
        m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
        m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
        m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
        m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
    }

    void Identity() {
        std::memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    static Matrix4 Translation(const Vector3& v) {
        Matrix4 result;
        result.m[12] = v.x;
        result.m[13] = v.y;
        result.m[14] = v.z;
        return result;
    }

    static Matrix4 Scale(const Vector3& v) {
        Matrix4 result;
        result.m[0] = v.x;
        result.m[5] = v.y;
        result.m[10] = v.z;
        return result;
    }

    static Matrix4 RotationX(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        Matrix4 result;
        result.m[5] = c;
        result.m[6] = s;
        result.m[9] = -s;
        result.m[10] = c;
        return result;
    }

    static Matrix4 RotationY(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        Matrix4 result;
        result.m[0] = c;
        result.m[2] = -s;
        result.m[8] = s;
        result.m[10] = c;
        return result;
    }

    static Matrix4 RotationZ(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        Matrix4 result;
        result.m[0] = c;
        result.m[1] = s;
        result.m[4] = -s;
        result.m[5] = c;
        return result;
    }

    static Matrix4 FromQuaternion(const Quaternion& q) {
        Quaternion n = q.Normalize();
        float xx = n.x * n.x;
        float yy = n.y * n.y;
        float zz = n.z * n.z;
        float xy = n.x * n.y;
        float xz = n.x * n.z;
        float yz = n.y * n.z;
        float wx = n.w * n.x;
        float wy = n.w * n.y;
        float wz = n.w * n.z;

        Matrix4 result;
        result.m[0] = 1 - 2 * (yy + zz);
        result.m[1] = 2 * (xy + wz);
        result.m[2] = 2 * (xz - wy);
        result.m[4] = 2 * (xy - wz);
        result.m[5] = 1 - 2 * (xx + zz);
        result.m[6] = 2 * (yz + wx);
        result.m[8] = 2 * (xz + wy);
        result.m[9] = 2 * (yz - wx);
        result.m[10] = 1 - 2 * (xx + yy);
        return result;
    }

    static Matrix4 Perspective(float fov, float aspect, float near, float far) {
        float f = 1.0f / std::tan(fov * 0.5f);
        Matrix4 result;
        result.m[0] = f / aspect;
        result.m[5] = f;
        result.m[10] = (far + near) / (near - far);
        result.m[11] = -1.0f;
        result.m[14] = (2.0f * far * near) / (near - far);
        result.m[15] = 0.0f;
        return result;
    }

    static Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far) {
        Matrix4 result;
        result.m[0] = 2.0f / (right - left);
        result.m[5] = 2.0f / (top - bottom);
        result.m[10] = -2.0f / (far - near);
        result.m[12] = -(right + left) / (right - left);
        result.m[13] = -(top + bottom) / (top - bottom);
        result.m[14] = -(far + near) / (far - near);
        return result;
    }

    static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
        Vector3 forward = (target - eye).Normalize();
        Vector3 right = forward.Cross(up).Normalize();
        Vector3 actualUp = right.Cross(forward);

        Matrix4 result;
        result.m[0] = right.x;
        result.m[1] = actualUp.x;
        result.m[2] = -forward.x;
        result.m[4] = right.y;
        result.m[5] = actualUp.y;
        result.m[6] = -forward.y;
        result.m[8] = right.z;
        result.m[9] = actualUp.z;
        result.m[10] = -forward.z;
        result.m[12] = -right.Dot(eye);
        result.m[13] = -actualUp.Dot(eye);
        result.m[14] = forward.Dot(eye);
        return result;
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i * 4 + j] = 0;
                for (int k = 0; k < 4; ++k) {
                    result.m[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
                }
            }
        }
        return result;
    }

    Vector4 operator*(const Vector4& v) const {
        return Vector4(
            m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
            m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
            m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
            m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w
        );
    }

    Matrix4 Transpose() const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i * 4 + j] = m[j * 4 + i];
            }
        }
        return result;
    }
};

} // namespace Luminary
