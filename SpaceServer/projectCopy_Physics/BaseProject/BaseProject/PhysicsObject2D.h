#pragma once

#include "BoundaryRect.h"
#include "SpriteBatch.h"
#include <typeinfo>
class PhysicsObject2D
{
public:	
	enum ColliderType{Rectangle, Circle, Triangle};
	PhysicsObject2D(int _id, Vector2D _position, CollisionRect* collider);		
	PhysicsObject2D(void);
	~PhysicsObject2D(void);

	bool DebugMode;

	ColliderType colliderType;
	CollisionRect* collider;
	
	bool movable;

	bool colliding;

	//Get the ID of this Physics Object
	int GetID(){return id;}

	//Set the rotational Inertia this object has, 0 is none
	void SetInertia(float newInertia){inertia = newInertia;}

	//Get the rotational Inertia this object has, 0 is none
	float GetInertia(){return inertia;}

	//Set the mass of this object;
	void SetMass(float newMass){mass = newMass;}

	//Get the mass of this object;
	float GetMass(){return mass;}

	static void setMapBounds(Vector2D* bounds);

	void AddImpulse(Vector2D _direction, float speed);

	//Set the amount of drag resistance, 0 is none 1 is max
	void SetDrag(float newDrag)	
	{
		if(drag>1)drag =1;			
		else if(drag < 0) drag =0;
		else drag = newDrag;
	}

	//Get the amount of drag resistance, 0 is none 1 is max
	float GetDrag(){return drag;}

	//Set the value of this object's rotation in radians
	void SetRotation(float newRotaton)
	{
		rotation = newRotaton;
	}

	//Get the value of this object's rotation in radians
	float GetRotation(){return rotation;}

	void Update(float timeStep);
	void Draw(SpriteBatch* _spriteBatch);

	//Positive torque is ccw and -torque is cw
	void AddTorque(float radius, float torque);

	//Add a force to the center mass 
	void AddForce(Vector2D direction, float force);

	Vector2D* GetPosition(){
		Vector2D temp = Vector2D(position);
		return &temp;	
	};

	//spring_position is where the spring is attached
	//springAtRest is distance where the spring no longer exerting force
	//SpringForce is the amount of strength the spring
	void SpringForce(Vector2D spring_position, float springAtRest, float SpringForce);
	
	//Set the velocity
	void SetVelocity(Vector2D vel){velocity = vel;}

	//Get the coefficient of restitution
	float GetCoefficient(){return coeff;}

	Vector2D* GetVelocity(){
		Vector2D temp = Vector2D(velocity);
		return &temp;	
	};
	float clamp(float n, float lower, float upper) {
	  return std::max(lower, std::min(n, upper));
	}
	float resolveNumber;
	Vector2D resolvedVelocity;
	Vector2D velocity;
	float rotation;
	Vector2D position;

	void PrintStuff()
	{
		//printf("position: x:%.2f y:%.2f velocity: x:%.2f y:%.2f acceleration: x:%.2f y:%.2f colliderType: %s",position.x,position.y, velocity.x, velocity.y, acceleration.x, acceleration.y, collider.name);
	};
	
protected:
	int id;
	
private:
	
	Vector2D acceleration;
	float rotVelocity;
	float rotAcceleration;
	float coeff;
	float mass;
	float inertia;	
	float drag;
};

