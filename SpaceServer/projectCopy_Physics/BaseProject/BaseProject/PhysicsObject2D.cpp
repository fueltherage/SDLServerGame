#include "PhysicsObject2D.h"


static Vector2D* mapBounds;
void PhysicsObject2D::setMapBounds(Vector2D* bounds){mapBounds = bounds;}

PhysicsObject2D::PhysicsObject2D(void)
{
}
PhysicsObject2D::PhysicsObject2D(int _id, Vector2D _position, CollisionRect* _collider)
{
	id = _id;	
	colliderType = Rectangle;
	position = _position;
	collider = _collider;
	collider->center = _position;
	colliding = false;
	resolveNumber =0;
	resolvedVelocity = Vector2D(0.0,0.0);
	velocity = Vector2D(0.0,0.0);
	acceleration = Vector2D(0.0,0.0);	
	mass =1;
	inertia =0;
	rotVelocity =0;
	rotAcceleration =0; 
	rotation =0;
	drag = 0;
	coeff = 1;
}
PhysicsObject2D::~PhysicsObject2D(void)
{


}
void PhysicsObject2D::Update(float timeStep)
{
	//printf("\n.%s.",typeid(collider).name());
	//std::string name = typeid(collider).name();

	if(collider->name == 'B')
	{
	  int a =0;
	}

	rotVelocity += rotAcceleration * timeStep;
	rotVelocity -= rotVelocity * drag;
	rotation += rotVelocity * timeStep + 0.5f* rotAcceleration * timeStep * timeStep;

	if(resolveNumber>0)
	velocity = resolvedVelocity / resolveNumber;

	resolvedVelocity = Vector2D(0);
	resolveNumber = 0;
	velocity += acceleration * timeStep;
	velocity -= velocity.normalized() * drag;	
	
	position += velocity * timeStep + 0.5f * acceleration * timeStep * timeStep;

	if(movable)
	{
	position.x = clamp(position.x, 45, mapBounds->x-45);
	position.y = clamp(position.y, 45, mapBounds->y-45);
	}
	//reset Accelerations otherwise forces never reset.
	rotAcceleration = 0;
	acceleration = Vector2D(0.0,0.0);

	collider->rotation = rotation;
	collider->center = position;
	collider->Colliding = colliding;
	colliding = false;

}
void PhysicsObject2D::AddTorque(float _radius, float _torque)
{
	if(inertia!=0)
	{
		rotAcceleration += _torque/ inertia;
	}
}
void PhysicsObject2D::AddForce(Vector2D _direction, float _force)
{
	acceleration += _direction.normalized() * _force;
}
void PhysicsObject2D::AddImpulse(Vector2D _direction, float speed)
{
	velocity += _direction * speed;
}
void PhysicsObject2D::SpringForce(Vector2D _position, float springAtRest, float Force)
{
	Vector2D difference = Vector2D(_position.x - position.x, _position.y - position.y);
	float springForce = difference.length() - springAtRest;
	acceleration += difference.normalized() * springForce;
}
void PhysicsObject2D::Draw(SpriteBatch* spriteBatch)
{
	collider->Draw(spriteBatch,id);
}


