#include "Player.h"

Player::Player(int health, int ammo, int ID, int teamNumber, Vector2D position)
	: PhysicsObject2D(ID, position, new CollisionRect(position, 20, 30)) {
	this->health = health;
	this->ammo = ammo;
	this->teamNumber = teamNumber;
}