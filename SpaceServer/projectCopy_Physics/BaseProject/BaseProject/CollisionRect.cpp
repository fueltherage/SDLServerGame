#include "CollisionRect.h"

CollisionRect::CollisionRect(void)
{   
	center=Vector2D(0,0);
	width = 1;
	height = 1;
	scale = Vector2D(1.0,1.0);
	c1 = Vector2D(-width/2.0f,height/2.0f);
	c2 = Vector2D(width/2.0f,height/2.0f);
	c3 = Vector2D(width/2.0f,-height/2.0f);
	c4 = Vector2D(-width/2.0f,-height/2.0f);		
}
CollisionRect::~CollisionRect(void)
{
}
CollisionRect::CollisionRect(Vector2D position, float _width, float _height)
{
	center = position;
	width = _width;
	height = _height; 
	scale = Vector2D(1.0,1.0);
	c1 = Vector2D(-width/2.0f,height/2.0f);
	c2 = Vector2D(width/2.0f,height/2.0f);
	c3 = Vector2D(width/2.0f,-height/2.0f);
	c4 = Vector2D(-width/2.0f,-height/2.0f);
	
}
void CollisionRect::Draw(SpriteBatch* spriteBatch,int id)
{
	SDL_Point* points = new SDL_Point[4]();
	points[0]=c1WorldSDLP();
	points[1]=c2WorldSDLP();
	points[2]=c3WorldSDLP();
	points[3]=c4WorldSDLP();
	SDL_Point* _center = new SDL_Point();
	_center->x = (int)center.x;
	_center->y = (int)center.y;
	spriteBatch->DrawLines(points,4,CreateColor(255,255,0,255),true);
	//spriteBatch->DrawString(0,convertInt(id),StringDrawMode::Blended,CreateColor(255,255,255,255),CreateColor(0,0,0,0),10,_center,0.0f,NULL,1.0,SDL_FLIP_NONE,1.0f);
}
