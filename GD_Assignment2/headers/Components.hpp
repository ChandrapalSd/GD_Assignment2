#pragma once
#include "Vec2.hpp"
#include "SFML/Graphics.hpp"
#include <cassert>
#include <chrono>

struct CTransform
{
	Vec2 pos;
	Vec2 velocityNormalized;
	float speed;

	CTransform(Vec2 pos, float speed, Vec2 velocityNormalized = {1,1})
		:pos(pos), velocityNormalized(velocityNormalized), speed(speed)
	{}
};

class CShape
{
public:
	sf::CircleShape shape;
	size_t pointCount = 30;
	float radius = 0.0;
	
	CShape() = delete;
	CShape(float radius, size_t pointCount=30)
		:radius(radius), pointCount(pointCount), shape(sf::CircleShape(radius, pointCount))
	{
		shape.setOrigin(radius, radius);
		assert(radius != 0 && "Radius must not be zero");
	}
};

struct CCollision
{
	float radius;

	CCollision(float radius)
		:radius(radius)
	{}
};

struct CInput
{
	bool wPressed = false;
	bool aPressed = false;
	bool sPressed = false;
	bool dPressed = false;
	bool leftMouse = false;
	bool rightMouse = false;
};

struct CScore
{
	int score = 0;
	sf::Text scoreText;
};

class CLifespan
{
public:
	size_t framesLeft;
	size_t startDisapearingAtFrames;

	CLifespan(const size_t framesLeft=30, const size_t startDisapearingAtFrames=120)
		:framesLeft(framesLeft), startDisapearingAtFrames(startDisapearingAtFrames)
	{}
};

class CGun
{
public:
	std::chrono::steady_clock::time_point lastSpawned;

	long long coolDownTime = 1e+9; // ns

	CGun(const long long coolDownTime_ms = 300 )
		: coolDownTime(coolDownTime_ms*1e+6),
		lastSpawned(std::chrono::high_resolution_clock::now())
	{}
};