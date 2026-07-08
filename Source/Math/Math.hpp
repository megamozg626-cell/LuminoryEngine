#pragma once

#include <cmath>

namespace Luminary::Math {

const float PI = 3.14159265359f;
const float TWO_PI = 6.28318530718f;
const float HALF_PI = 1.57079632679f;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

inline float Degrees(float radians) {
    return radians * RAD_TO_DEG;
}

inline float Radians(float degrees) {
    return degrees * DEG_TO_RAD;
}

inline float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

inline float Saturate(float value) {
    return Clamp(value, 0.0f, 1.0f);
}

} // namespace Luminary::Math
