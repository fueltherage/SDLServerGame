#pragma once
#include "PhysicsObject2D.h";
class Player : PhysicsObject2D {

public:
	int maxHealth;
	int health;
	int maxAmmo;
	int ammo;
	int teamNumber;

	Player(int health, int ammo, int ID, int teamNumber, Vector2D position);
	~Player();
private:

};