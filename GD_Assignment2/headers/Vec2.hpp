#pragma once

class Vec2
{
public:
	Vec2(double x = 0.0, double y = 0.0);
	double x{ 0 };
	double y{ 0 };
public:
	[[nodiscard]] bool operator== (const Vec2& other) const noexcept;
	[[nodiscard]] bool operator!= (const Vec2& other) const noexcept;
	[[nodiscard]] Vec2 operator+  (const Vec2& other) const noexcept;
	[[nodiscard]] Vec2 operator-  (const Vec2& other) const noexcept;
	[[nodiscard]] Vec2 operator*  (const double scale) const noexcept;
	[[nodiscard]] Vec2 operator/  (const double scale) const noexcept;

	void operator+= (const Vec2& other) noexcept;
	void operator-= (const Vec2& other) noexcept;
	void operator*= (const double scale) noexcept;
	void operator/= (const double scale) noexcept;

	void normalize() noexcept;
	[[nodiscard]] static Vec2 normalize(const Vec2& vec) noexcept;
	[[nodiscard]] double length() const noexcept;
};

