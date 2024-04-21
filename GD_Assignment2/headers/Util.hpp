#include <random>

//inline float randInRange(float mn, float mx) {
//	return (((float)rand() / RAND_MAX) * (mx - mn) + mn);
//}

template <typename T>
inline T randInRange(T mn, T mx) {
	return (T)(((double)rand() / RAND_MAX)* (mx - mn) + mn);
}
