#include "BoundaryRect.h"


BoundaryRect::BoundaryRect(void)
{
}
BoundaryRect::BoundaryRect(int width, int height,int cellWidth,int cellHeight, Vector2D _position)
{
	name = 'B';
	center = _position;	
	c1 = Vector2D(width*cellWidth, 0);
	c2 = Vector2D(width*cellWidth, height*cellHeight);
	c3 = Vector2D(0, height * cellHeight);
	rotation =0;
	scale = Vector2D(1.0,1.0);
	//c4 is useless in this class.5

}
BoundaryRect::~BoundaryRect(void)
{
}
void BoundaryRect::Draw(SpriteBatch* spriteBatch)
{
	SDL_Point* points = new SDL_Point[4]();
	
	points[0]= position();
	points[1]=c1WorldSDLP();
	points[2]=c2WorldSDLP();
	points[3]=c3WorldSDLP();	
	spriteBatch->DrawLines(points,4,CreateColor(255,255,0,255),true);
}