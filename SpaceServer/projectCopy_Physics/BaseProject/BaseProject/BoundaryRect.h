#pragma once
#include "CollisionRect.h"

class BoundaryRect : public CollisionRect
{
public:
	BoundaryRect(void);
	BoundaryRect(int width, int height,int cellWidth,int cellHeight, Vector2D _position);
	~BoundaryRect(void);

	void Draw(SpriteBatch* spriteBatch);

};

