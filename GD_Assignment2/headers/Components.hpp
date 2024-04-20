#pragma once
#include "Vec2.hpp"

struct CTransform
{
	Vec2 pos;
	Vec2 speed;
	Vec2 scale;
	double angle;
};

struct CShape
{
};

struct CCollision
{
	double radius;
};

struct CInput
{
};

struct CScore
{
	int score;
};

struct CLifespan
{
};

