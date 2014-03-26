#pragma once
#include "Vector2D.h" 
#include "SpriteBatch.h"
#include <string>
#include <sstream>

//#include "CollisionObject.h"
class CollisionRect
{
public:
	CollisionRect(void);
	~CollisionRect(void);	
	CollisionRect(Vector2D _center, float _width, float _height);
	//the world position of each corner
	// Rotation Matrix CounterClockWise
	// [x,y] * [Cos0 - Sin0] 
	//		   [Sin0 + Cos0]
	
	//World position of corner c1
	Vector2D c1World() { return Vector2D((cos(rotation)*c1.x*scale.x - sin(rotation)*c1.y*scale.y )+center.x
									    ,(sin(rotation)*c1.x*scale.x + cos(rotation)*c1.y*scale.y)+center.y);}
	//World position of corner c2
	Vector2D c2World() { return Vector2D((cos(rotation)*c2.x*scale.x - sin(rotation)*c2.y*scale.y)+center.x 
									    ,(sin(rotation)*c2.x*scale.x + cos(rotation)*c2.y*scale.y)+center.y);}
	//World position of corner c3
	Vector2D c3World() { return Vector2D((cos(rotation)*c3.x*scale.x - sin(rotation)*c3.y*scale.y)+center.x 
									    ,(sin(rotation)*c3.x*scale.x + cos(rotation)*c3.y*scale.y)+center.y);}
	//World position of corner c4
	Vector2D c4World() { return Vector2D((cos(rotation)*c4.x*scale.x - sin(rotation)*c4.y*scale.y)+center.x 
									    ,(sin(rotation)*c4.x*scale.x + cos(rotation)*c4.y*scale.y)+center.y);}
	
	SDL_Point c1WorldSDLP() 
	{ 		
		point1.x = (int)((cos(rotation)*c1.x*scale.x - sin(rotation)*c1.y*scale.y )+center.x);
		point1.y = (int)((sin(rotation)*c1.x*scale.x + cos(rotation)*c1.y*scale.y )+center.y);
		return point1;
	}
	SDL_Point c2WorldSDLP() 
	{ 		
		point2.x = (int)((cos(rotation)*c2.x*scale.x - sin(rotation)*c2.y*scale.y )+center.x);
		point2.y = (int)((sin(rotation)*c2.x*scale.x + cos(rotation)*c2.y*scale.y )+center.y);
		return point2;
	}
	SDL_Point c3WorldSDLP() 
	{ 
		
		point3.x = (int)((cos(rotation)*c3.x*scale.x - sin(rotation)*c3.y*scale.y )+center.x);
		point3.y = (int)((sin(rotation)*c3.x*scale.x + cos(rotation)*c3.y*scale.y )+center.y);
		return point3;
	}
	SDL_Point c4WorldSDLP() 
	{		
		point4.x = (int)((cos(rotation)*c4.x*scale.x - sin(rotation)*c4.y*scale.y )+center.x);
		point4.y = (int)((sin(rotation)*c4.x*scale.x + cos(rotation)*c4.y*scale.y )+center.y);
		return point4;
	}
	std::string convertInt(int number)
	{
	   std::stringstream ss;//create a stringstream
	   ss << number;//add number to the stream
	   return ss.str();//return a string with the contents of the stream
	}

	//Amount of scale from base shape	
	Vector2D scale;
	//Rotation in Radians	
	float rotation;
	Vector2D center;	
	float GetWidth(){return width;}
	float GetHeight(){return height;}
	void Draw(SpriteBatch* spriteBatch,int id);
	
private:
	void Rotate(float _rotation);
	
	//Local Position
	Vector2D c1,c2,c3,c4;
	SDL_Point point1;
	SDL_Point point2;
	SDL_Point point3;
	SDL_Point point4;
		
	float width;
	float height;
};

