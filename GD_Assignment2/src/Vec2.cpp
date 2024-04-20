#include "Vec2.hpp"
#include <cmath>

inline bool Vec2::operator==(const Vec2& other) const noexcept
{
	return x == other.x && y == other.y;
}

inline bool Vec2::operator!=(const Vec2& other) const noexcept
{
	return x != other.x || y != other.y;
}

inline Vec2 Vec2::operator+(const Vec2& other) const noexcept
{
	return {x+other.x, y+other.y};
}

inline Vec2 Vec2::operator-(const Vec2& other) const noexcept
{
	return {x-other.x, y-other.y};
}

inline Vec2 Vec2::operator*(const double scale) const noexcept
{
	return {x*scale, y*scale};
}

inline Vec2 Vec2::operator/(const double scale) const noexcept
{
	return {x/scale, y/scale};
}

inline void Vec2::operator+=(const Vec2& other) noexcept
{
	x += other.x;
	y += other.y;
}

inline void Vec2::operator-=(const Vec2& other) noexcept
{
	x -= other.x;
	y -= other.y;
}

inline void Vec2::operator*=(const double scale) noexcept
{
	x *= scale;
	y *= scale;
}

inline void Vec2::operator/=(const double scale) noexcept
{
	x /= scale;
	y /= scale;
}

inline void Vec2::normalize() noexcept
{
	*this /= length();
}

inline Vec2 Vec2::normalize(const Vec2& vec) noexcept
{
	return vec / vec.length();
}

inline double Vec2::length() const noexcept
{
	return sqrt(x * x + y * y);
}



