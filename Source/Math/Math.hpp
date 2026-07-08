#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include <cmath>
#include <algorithm>

namespace Luminary::Math {

const float PI = 3.14159265359f;
const float TWO_PI = 6.28318530718f;
const float HALF_PI = 1.57079632679f;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;
const float EPSILON = 1e-6f;

inline float Radians(float degrees) {
    return degrees * DEG_TO_RAD;
}

inline float Degrees(float radians) {
    return radians * RAD_TO_DEG;
}

inline float Clamp(float value, float min, float max) {
    return std::clamp(value, min, max);
}

inline float Lerp(float a, float b, float t) {
    return a + (b - a) * Clamp(t, 0.0f, 1.0f);
}

inline float Smoothstep(float edge0, float edge1, float x) {
    float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

inline float Sin(float x) {
    return std::sin(x);
}

inline float Cos(float x) {
    return std::cos(x);
}

inline float Tan(float x) {
    return std::tan(x);
}

inline float Abs(float x) {
    return std::abs(x);
}

inline float Sqrt(float x) {
    return std::sqrt(x);
}

inline float Pow(float x, float y) {
    return std::pow(x, y);
}

inline float Max(float a, float b) {
    return std::max(a, b);
}

inline float Min(float a, float b) {
    return std::min(a, b);
}

inline bool IsZero(float value, float epsilon = EPSILON) {
    return Abs(value) < epsilon;
}

inline bool Equals(float a, float b, float epsilon = EPSILON) {
    return Abs(a - b) < epsilon;
}

} // namespace Luminary::Math
