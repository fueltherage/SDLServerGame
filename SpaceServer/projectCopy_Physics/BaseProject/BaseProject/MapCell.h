#pragma once
#include <vector>
#include <iostream>
#include "PhysicsObject2D.h"
#include "SDL_rect.h"
#include "SpriteBatch.h"

//#define vector std::vector; //why doesnt this work?

class MapCell
{
public:
	MapCell(int TextureIndex, int _x, int _y, float gridSpacing, SpriteBatch* spriteBatch);
	MapCell(void);
	~MapCell(void);
	std::vector<PhysicsObject2D*> RegisteredObjects;
	std::vector<PhysicsObject2D*> RegisteredStaticObjects;
	SDL_Rect cellDestRect;
	SDL_Rect cellSourceRect;
	int Texture;
	void Add(PhysicsObject2D* _object);
	void AddStaticObject(PhysicsObject2D* _object);
	void Remove(PhysicsObject2D* _object); 
	bool Has(PhysicsObject2D* _object);
	bool Active(){
		if((RegisteredObjects.size()+RegisteredStaticObjects.size())>1 && RegisteredObjects.size()!=0)
		{return true;}
		 else return false;	};	
	int Count(){
		return RegisteredObjects.size()+RegisteredStaticObjects.size();

	};
	std::string convertInt(int number)
	{
	   std::stringstream ss;//create a stringstream
	   ss << number;//add number to the stream
	   return ss.str();//return a string with the contents of the stream
	};
	virtual void Update(float gameTime);
		void Draw(SpriteBatch* spriteBatch);
	float clamp(float n, float lower, float upper) {
	  return std::max(lower, std::min(n, upper));
	}
	
private:
	int x;
	int y;
	Vector2D position;//position will be top left of cell
};

