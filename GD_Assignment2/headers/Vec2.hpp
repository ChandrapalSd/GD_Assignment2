#pragma once
#include <cstdlib>

class Vec2
{
public:
	Vec2(float x = 0.0, float y = 0.0);
	float x{ 0 };
	float y{ 0 };
public:
	[[nodiscard]] bool operator== (const Vec2& other) const noexcept;
	[[nodiscard]] bool operator!= (const Vec2& other) const noexcept;
	[[nodiscard]] Vec2 operator+  (const Vec2& other) const noexcept;
	[[nodiscard]] Vec2 operator-  (const Vec2& other) const noexcept;
	[[nodiscard]] Vec2 operator*  (const float scale) const noexcept;
	[[nodiscard]] Vec2 operator/  (const float scale) const noexcept;

	void operator+= (const Vec2& other) noexcept;
	void operator-= (const Vec2& other) noexcept;
	void operator*= (const float scale) noexcept;
	void operator/= (const float scale) noexcept;

	void normalize() noexcept;
	[[nodiscard]] static Vec2 normalize(const Vec2& vec) noexcept;
	[[nodiscard]] float length() const noexcept;
	static float dist(const Vec2 a, const Vec2 b) {
		return std::abs( Vec2(a.x - b.x, a.y - b.y).length() );
	}
};

