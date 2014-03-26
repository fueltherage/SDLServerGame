#include "CollisionPair.h"



CollisionPair::CollisionPair(PhysicsObject2D* _object1, PhysicsObject2D* _object2)
{
	object1 = _object1;
	object2 = _object2;
}
CollisionPair::CollisionPair(void)
{
}
CollisionPair::~CollisionPair(void)
{
}
