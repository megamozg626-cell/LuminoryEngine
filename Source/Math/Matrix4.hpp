#pragma once

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include <cmath>

namespace Luminary {

class Matrix4 {
public:
    float m[16];

    Matrix4() {
        // Identity matrix
        m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f;  m[12] = 0.0f;
        m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f;  m[13] = 0.0f;
        m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
        m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i * 4 + j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
                }
            }
        }
        return result;
    }

    static Matrix4 Translation(const Vector3& pos) {
        Matrix4 result;
        result.m[12] = pos.x;
        result.m[13] = pos.y;
        result.m[14] = pos.z;
        return result;
    }

    static Matrix4 Scale(const Vector3& scale) {
        Matrix4 result;
        result.m[0] = scale.x;
        result.m[5] = scale.y;
        result.m[10] = scale.z;
        return result;
    }

    static Matrix4 FromQuaternion(const Quaternion& q) {
        Matrix4 result;
        float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
        float xx2 = q.x * x2, yy2 = q.y * y2, zz2 = q.z * z2;
        float xy2 = q.x * y2, xz2 = q.x * z2, yz2 = q.y * z2;
        float wx2 = q.w * x2, wy2 = q.w * y2, wz2 = q.w * z2;

        result.m[0] = 1.0f - (yy2 + zz2);
        result.m[1] = xy2 + wz2;
        result.m[2] = xz2 - wy2;
        result.m[4] = xy2 - wz2;
        result.m[5] = 1.0f - (xx2 + zz2);
        result.m[6] = yz2 + wx2;
        result.m[8] = xz2 + wy2;
        result.m[9] = yz2 - wx2;
        result.m[10] = 1.0f - (xx2 + yy2);
        return result;
    }

    static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
        Vector3 f = (center - eye).Normalize();
        Vector3 s = f.Cross(up).Normalize();
        Vector3 u = s.Cross(f);

        Matrix4 result;
        result.m[0] = s.x;
        result.m[4] = s.y;
        result.m[8] = s.z;
        result.m[1] = u.x;
        result.m[5] = u.y;
        result.m[9] = u.z;
        result.m[2] = -f.x;
        result.m[6] = -f.y;
        result.m[10] = -f.z;
        result.m[12] = -s.Dot(eye);
        result.m[13] = -u.Dot(eye);
        result.m[14] = f.Dot(eye);
        return result;
    }

    static Matrix4 Perspective(float fov, float aspect, float near, float far) {
        float f = 1.0f / std::tan(fov * Math::DEG_TO_RAD * 0.5f);
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
};

} // namespace Luminary
