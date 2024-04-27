#include "Vec2.hpp"
#include <cmath>

Vec2::Vec2(float x, float y)
	:x(x), y(y)
{
}

bool Vec2::operator==(const Vec2& other) const noexcept
{
	return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other) const noexcept
{
	return x != other.x || y != other.y;
}

Vec2 Vec2::operator+(const Vec2& other) const noexcept
{
	return {x+other.x, y+other.y};
}

Vec2 Vec2::operator-(const Vec2& other) const noexcept
{
	return {x-other.x, y-other.y};
}

Vec2 Vec2::operator*(const float scale) const noexcept
{
	return {x*scale, y*scale};
}

Vec2 Vec2::operator/(const float scale) const noexcept
{
	return {x/scale, y/scale};
}

void Vec2::operator+=(const Vec2& other) noexcept
{
	x += other.x;
	y += other.y;
}

void Vec2::operator-=(const Vec2& other) noexcept
{
	x -= other.x;
	y -= other.y;
}

void Vec2::operator*=(const float scale) noexcept
{
	x *= scale;
	y *= scale;
}

void Vec2::operator/=(const float scale) noexcept
{
	x /= scale;
	y /= scale;
}

void Vec2::normalize() noexcept
{
	float len = length();
	*this = len > 0.01 ? *this/length() : 0;
}

Vec2 Vec2::normalize(const Vec2& vec) noexcept
{
	float len = vec.length();
	return len > 0.01 ? vec / len : 0;
}

float Vec2::length() const noexcept
{
	return sqrt(x * x + y * y);
}



