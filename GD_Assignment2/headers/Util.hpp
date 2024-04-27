#pragma once
#include <random>

template <typename T>
inline T randInRange(T mn, T mx) {
	return (T)(((float)rand() / RAND_MAX)* (mx - mn) + mn);
}
