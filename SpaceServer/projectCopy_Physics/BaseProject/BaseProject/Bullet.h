#pragma once
#include "PhysicsObject2D.h"
class Bullet : PhysicsObject2D
{
public:
	Bullet(int teamNumber);
	~Bullet(void);
	int teamNumber;
	int damage;
private:
};