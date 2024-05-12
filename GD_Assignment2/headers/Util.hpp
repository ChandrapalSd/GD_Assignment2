#pragma once
#include <random>

constexpr float TAU = 6.283185307179586f;
constexpr float PI = 3.141592653589793f;

template <typename T>
inline T randInRange(T mn, T mx) {
	return (T)(((float)rand() / RAND_MAX)* (mx - mn) + mn);
}

// t01 is between 0.0 and 1.0
template <typename T>
T lerp(T mn, T mx, float t01) {
	t01 = std::clamp(t01, 0.0f, 1.0f);
	return (mx - mn) * t01 + mn;
}