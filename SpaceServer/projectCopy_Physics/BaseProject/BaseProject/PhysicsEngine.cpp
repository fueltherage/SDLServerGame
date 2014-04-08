#include "PhysicsEngine.h"
static PhysicsEngine* instance;

PhysicsEngine* PhysicsEngine::GetInstance()
{
	//if no instance exists then make one and return it
	if(instance == nullptr)instance = new PhysicsEngine();
	//otherwise return the instance.
	return instance;
}
PhysicsEngine::PhysicsEngine(void)
{
}
PhysicsEngine::~PhysicsEngine(void)
{
}
//Registers objects by pairing them together to be checked for collision
void PhysicsEngine::RegisterPairedCollision(PhysicsObject2D* _object1, PhysicsObject2D* _object2)
{
	//Note Redundant looping
	//This method has to loop through objects that might have no relation to oneanother 
	//making checking to see if the objects are paired a waste of time
	for(unsigned int i = 0; i < pairedCollisions.size(); i++)
	{
		if(pairedCollisions[i]->object1->GetID() == _object1->GetID() && pairedCollisions[i]->object2->GetID() == _object2->GetID() ||
			pairedCollisions[i]->object2->GetID() == _object1->GetID() && pairedCollisions[i]->object1->GetID() == _object2->GetID())
		{
			return;
		}
	}
	pairedCollisions.push_back(new CollisionPair(_object1,_object2));
}
//PhysicsEngine Update must be called each frame to clear pairedCollision buffer
void PhysicsEngine::Update(float gameTime)
{
	for (unsigned int i=0; i<pairedCollisions.size(); i++)
	{
		CollisionInfo collisionInfo = DetectCollision(pairedCollisions[i]->object1,pairedCollisions[i]->object2);
		if(collisionInfo.isColliding) {		
			CollisionResponse(pairedCollisions[i]->object1,pairedCollisions[i]->object2, collisionInfo);
		}
	}
	//pairedCollisionCount=0;
	pairedCollisions.clear(); //Clear the collision registerer after collisions were checked
}

CollisionInfo PhysicsEngine::DetectCollision(PhysicsObject2D* _object1, PhysicsObject2D* _object2)
{
	//Check to see if both objects are Collider Type Rect
	//if( dynamic_cast<CollisionRect>(_object1->collider ) != nullptr && dynamic_cast<CollisionRect>(_object1->collider ) != nullptr)

	/*std::string name1 = typeid(&_object1->collider).name();
	std::string name2 = typeid(&_object2->collider).name();*/

	CollisionInfo collisionInfo = CollisionInfo();
	collisionInfo.isColliding = false;
	collisionInfo.overlap = Vector2D(0);

	std::vector<Vector2D>Edges;
	Edges.push_back(Vector2D(_object1->collider->c1World().x-_object1->collider->c2World().x,
								_object1->collider->c1World().y-_object1->collider->c2World().y));
	Edges.push_back(Vector2D(_object1->collider->c2World().x-_object1->collider->c3World().x,
								_object1->collider->c2World().y-_object1->collider->c3World().y));
	Edges.push_back(Vector2D(_object2->collider->c1World().x-_object2->collider->c2World().x,
								_object2->collider->c1World().y-_object2->collider->c2World().y));
	Edges.push_back(Vector2D(_object2->collider->c2World().x-_object2->collider->c3World().x,
								_object2->collider->c2World().y-_object2->collider->c3World().y));

	for (int i = 0; i < Edges.size(); i++)
	{
		Vector2D axis = Edges[i];
		axis = axis.normalized();
		axis.Perpendicular();

		Vector2D _object1Max = Vector2D();
		Vector2D _object1Min = Vector2D();
		Vector2D _object2Max = Vector2D();
		Vector2D _object2Min = Vector2D();
		float _object1MaxDot;
		float _object1MinDot;
		float _object2MaxDot;
		float _object2MinDot;
		GetMinAndMax(_object1, _object1Min, _object1Max, _object1MinDot, _object1MaxDot, axis);
		GetMinAndMax(_object2, _object2Min, _object2Max, _object2MinDot, _object2MaxDot, axis);

		if(_object1MinDot>_object2MaxDot | _object1MaxDot<_object2MinDot) {
			collisionInfo.isColliding = false;
			return collisionInfo;
		}else{
			Vector2D overlap1 = axis * (_object1Min.DotProduct(axis) - _object2Max.DotProduct(axis));
			Vector2D overlap2 = axis * (_object1Max.DotProduct(axis) - _object2Min.DotProduct(axis));
			if(i==0 | overlap1.length()<collisionInfo.overlap.length() | overlap2.length()<collisionInfo.overlap.length()) {
				if (overlap1.length() < overlap2.length()) {
					collisionInfo.overlap = overlap1;
					if (i<2) {
						collisionInfo.collisionPoint = _object2Max;
					} else {
						collisionInfo.collisionPoint = _object1Min;
					}
					collisionInfo.collisionPoint -= collisionInfo.overlap;
				} else {
					collisionInfo.overlap = overlap2;
					if (i<2) {
						collisionInfo.collisionPoint = _object2Min;
					} else {
						collisionInfo.collisionPoint = _object1Max;
					}
				}
			}
		}
	}

	if(collisionInfo.overlap.length()>0.00001)
		collisionInfo.isColliding=true;
	return collisionInfo;	
}
void PhysicsEngine::GetMinAndMax(PhysicsObject2D* _object, Vector2D &_min, Vector2D &_max, float &_minDot, float &_maxDot, Vector2D _axis) {
	std::vector<Vector2D> corners;
	corners.push_back(_object->collider->c1World());
	corners.push_back(_object->collider->c2World());
	corners.push_back(_object->collider->c3World());
	corners.push_back(_object->collider->c4World());

	for (int i = 0; i < corners.size(); i++)
	{
		float dot = corners[i].DotProduct(_axis);
		if(i==0 | dot<_minDot) {
			_minDot = dot;
			_min = corners[i];
		}
		if(i==0 | dot>_maxDot) {
			_maxDot = dot;
			_max = corners[i];
		}
	}
}

void PhysicsEngine::CollisionResponse(PhysicsObject2D* _object1, PhysicsObject2D* _object2, CollisionInfo collisionInfo)
{
	//Scotts formula for collision response.... Might only work on balls
	/*
	V1Prime = (m1  - e*m2)*V1 + (1+e)*m2*v2
				---------------------------
						m1+m2

	V2Prime = (m2  - e*m1)*V2 + (1+e)*m1*v1
				---------------------------
						m1+m2
	*/

	Vector2D object1V = Vector2D(_object1->velocity);
	Vector2D object2V = Vector2D(_object2->velocity);
	
	if(_object1->movable&&_object2->movable) {	
		_object1->velocity = Vector2D(((_object1->GetMass() - _object1->GetCoefficient()*_object2->GetMass())*object1V 
						+ (1+_object1->GetCoefficient())*_object2->GetMass()*object2V)/(_object1->GetMass()+_object2->GetMass()));

		_object2->velocity = Vector2D(((_object2->GetMass() - _object2->GetCoefficient()*_object1->GetMass())*object2V 
						+ (1+_object2->GetCoefficient())*_object1->GetMass()*object1V)/(_object2->GetMass()+_object1->GetMass()));

		_object1->position -= collisionInfo.overlap * 0.5f;	
		_object2->position += collisionInfo.overlap * 0.5f;

	}
	else if(_object2->movable && !_object1->movable) {
		//printf("\n\n velocity before: x:%.2f y:%.2f", _object2->velocity.x,_object2->velocity.y);
		
		Vector2D normal = _object1->position - _object2->position;		
		
		if(abs(normal.x) > abs(normal.y))
		{
			normal.y=0;
		}else normal.x =0;

		normal = normal/normal.length();
		//printf("\nnormal x:%.2f y:%.2f", normal.x, normal.y);
		Vector2D boundary = Vector2D(-normal.y, normal.x);
	

		//printf("\nboundary x:%.2f y:%.2f", boundary.x, boundary.y);

		float boundVel = Dot(&boundary, &_object2->velocity);
		float normalVel = -1*Dot(&normal, &_object2->velocity);

		_object2->resolvedVelocity += normal * normalVel + boundary * boundVel;
		_object2->resolveNumber++;
		//_object2->velocity = normal * normalVel + boundary * boundVel;
		//printf("\nvelocity after: x:%.2f y:%.2f", _object1->velocity.x,_object1->velocity.y);
		_object2->position -= Dot(&normal,&collisionInfo.overlap)*(collisionInfo.overlap/collisionInfo.overlap.length());

		
	}
	else if(_object1->movable && !_object2->movable) {	

		//printf("\n\nvelocity before: x:%.2f y:%.2f", _object1->velocity.x,_object1->velocity.y);
		
		Vector2D normal = _object2->position - _object1->position;		
		if(abs(normal.x) > abs(normal.y))
		{
			normal.y=0;
		}else normal.x =0;

		normal = normal/normal.length();
		//printf("\nnormal x:%.2f y:%.2f", normal.x, normal.y);
		Vector2D boundary = Vector2D(-normal.y, normal.x);
	

		//printf("\nboundary x:%.2f y:%.2f", boundary.x, boundary.y);

		float boundVel = Dot(&boundary, &_object1->velocity);
		float normalVel = -1*Dot(&normal, &_object1->velocity);

		_object1->resolvedVelocity += normal * normalVel + boundary * boundVel;
		_object1->resolveNumber++;
		//_object1->velocity = normal * normalVel + boundary * boundVel;
		//printf("\nvelocity after: x:%.2f y:%.2f", _object1->velocity.x,_object1->velocity.y);
		_object1->position -= Dot(&normal,&collisionInfo.overlap)*collisionInfo.overlap/collisionInfo.overlap.length();
	}

}
