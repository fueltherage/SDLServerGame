#pragma once
#include "TileEngine.h"
#include "CollisionRect.h"

class TileMapCollisionScanner
{
public:


	std::vector<CollisionRect*>rects;
	

	TileMapCollisionScanner(TileEngine* tiles);
	TileMapCollisionScanner(void);
	


	~TileMapCollisionScanner(void);
};

