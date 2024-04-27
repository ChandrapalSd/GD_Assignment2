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
	double len = length();
	*this = len > 0.01 ? *this/length() : 0;
}

Vec2 Vec2::normalize(const Vec2& vec) noexcept
{
	double len = vec.length();
	return len > 0.01 ? vec / len : 0;
}

double Vec2::length() const noexcept
{
	return sqrt(x * x + y * y);
}



