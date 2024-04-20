#pragma once

class Vec2
{
public:
	double x{ 0 };
	double y{ 0 };
public:
	[[nodiscard]] inline bool operator== (const Vec2& other) const noexcept;
	[[nodiscard]] inline bool operator!= (const Vec2& other) const noexcept;
	[[nodiscard]] inline Vec2 operator+  (const Vec2& other) const noexcept;
	[[nodiscard]] inline Vec2 operator-  (const Vec2& other) const noexcept;
	[[nodiscard]] inline Vec2 operator*  (const double scale) const noexcept;
	[[nodiscard]] inline Vec2 operator/  (const double scale) const noexcept;

	[[nodiscard]] inline void operator+= (const Vec2& other) noexcept;
	[[nodiscard]] inline void operator-= (const Vec2& other) noexcept;
	[[nodiscard]] inline void operator*= (const double scale) noexcept;
	[[nodiscard]] inline void operator/= (const double scale) noexcept;

	inline void normalize() noexcept;
	[[nodiscard]] inline static Vec2 normalize(const Vec2& vec) noexcept;
	[[nodiscard]] inline double length() const noexcept;
};

