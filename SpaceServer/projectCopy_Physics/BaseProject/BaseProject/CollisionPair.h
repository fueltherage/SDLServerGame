#pragma once
#include "PhysicsObject2D.h"
class CollisionPair
{
public:
	
	CollisionPair(void);
	~CollisionPair(void);
	CollisionPair(PhysicsObject2D* _object1, PhysicsObject2D* _object2);
	PhysicsObject2D* object1;
	PhysicsObject2D* object2;
	
private:
	
};

