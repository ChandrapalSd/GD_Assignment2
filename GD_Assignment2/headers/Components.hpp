#pragma once
#include "Vec2.hpp"
#include "SFML/Graphics.hpp"
#include <cassert>

struct CTransform
{
	Vec2 pos;
	Vec2 velocityNormalized;
	double speed;

	CTransform(Vec2 pos, double speed, Vec2 velocityNormalized = {1,1})
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
		:radius(radius), pointCount(30), shape(sf::CircleShape(radius, pointCount))
	{
		assert(radius != 0 && "Radius must not be zero");
	}
};

struct CCollision
{
	double radius;

	CCollision(double radius)
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
};

struct CLifespan
{
	size_t framesLeft;
};

