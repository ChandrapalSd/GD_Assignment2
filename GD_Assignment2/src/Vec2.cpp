#include "Vec2.hpp"
#include <cmath>

Vec2::Vec2(double x, double y)
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

Vec2 Vec2::operator*(const double scale) const noexcept
{
	return {x*scale, y*scale};
}

Vec2 Vec2::operator/(const double scale) const noexcept
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

void Vec2::operator*=(const double scale) noexcept
{
	x *= scale;
	y *= scale;
}

void Vec2::operator/=(const double scale) noexcept
{
	x /= scale;
	y /= scale;
}

void Vec2::normalize() noexcept
{
	*this /= length();
}

Vec2 Vec2::normalize(const Vec2& vec) noexcept
{
	return vec / vec.length();
}

double Vec2::length() const noexcept
{
	return sqrt(x * x + y * y);
}



