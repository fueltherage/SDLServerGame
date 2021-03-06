#pragma once
#include "Vector2D.h"

struct CollisionInfo
{
	Vector2D collisionAxis;
	Vector2D collisionPoint;
	Vector2D overlap;
	bool isColliding;
};