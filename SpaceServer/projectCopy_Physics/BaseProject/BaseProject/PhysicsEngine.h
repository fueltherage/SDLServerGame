#pragma once
#include "CollisionPair.h"
#include <vector>
#include <typeinfo.h>
#include "CollisionInfo.h"
class PhysicsEngine
{
public:
	PhysicsEngine(void);
	~PhysicsEngine(void);
	static PhysicsEngine* GetInstance();
	//Detect if two objects are colliding
	CollisionInfo DetectCollision(PhysicsObject2D* _object1, PhysicsObject2D* _object2);
	
	//Register two objects to be paired for collision
	void RegisterPairedCollision(PhysicsObject2D* _object1, PhysicsObject2D* _object2);
	//Calculates the response of collisions
	void CollisionResponse(PhysicsObject2D* _object1, PhysicsObject2D* _object2, CollisionInfo collisionInfo);
	void CollisionResponse2(PhysicsObject2D* _object1, PhysicsObject2D* _object2, CollisionInfo collisionInfo);
	void Update(float gameTime);
	//pairedCollisions is a buffer to hold objects that could be colliding.
	 std::vector<CollisionPair*> pairedCollisions;
	 //Calculate Angle between two vectors
	float DotProduct(Vector2D *vec1, Vector2D *vec2);
	float Dot(Vector2D *vec1, Vector2D *vec2){return (vec1->x*vec2->x)+(vec1->y*vec2->y);}
	
	//Normalize a vector
	void Normalize(Vector2D &vec);
private:
	void GetMinAndMax(PhysicsObject2D* _object, Vector2D &_min, Vector2D &_max, float &_minDot, float &_maxDot, Vector2D _axis);
};

