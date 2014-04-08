#include "TileMapCollisionScanner.h"
//coders: Wes allard
TileMapCollisionScanner::TileMapCollisionScanner(void)
{
}
TileMapCollisionScanner::~TileMapCollisionScanner(void)
{
}
	/*Pseudo code 	
	Tiles will combine and chain together to form rectangles
	this algorym will place a rectangle that stretches its height or width, placing shapes that are either 1 block wide, or 1 block height but length = n * cellSpacing
	 loop for map.cellsWide 
	   Horizontal detection: if n = 1 and n+1 = 1 .... until n+x = 0; Do countX++
	   loop for map.cellsHeight
	   Verticle detection: if n = 1 and n+1 = 1 .... until n+y = 0; Do countY++
		
	   vector list rectangles += new rectangle( width = cellWidth*countX, height = cellHeight*countY, position.x = n *cellWidth, position.y = n *cellHeight)	
	*/
TileMapCollisionScanner::TileMapCollisionScanner(TileEngine* tiles)
{
	//Horizontal 
	std::vector<int> leftOverTiles;

	int tileWidth = tiles->getTileWidth();
	int tileHeight = tiles->getTileHeight();
	int GridWidth = tiles->getGridHeight();
	int GridHeight = tiles->getGridWidth();

	int nextTile;
	int currentTile;
	int countX=0;
	int countY=0;
	Vector2D rectPosition;

	for each(int number in tiles->levelVec)
	{		
		leftOverTiles.push_back(number);		
	}
	bool RectanglePosSet = false;

	//Horizontal Rectangle creation.
	for(int y=0; y<tiles->getGridHeight();y++)
	{
		for(int x=0; x<tiles->getGridWidth();x++)
		{			
			currentTile = tiles->levelVec[x+y*GridWidth];			
			nextTile = tiles->levelVec[x+1+y*GridWidth];
			if(x<tiles->getGridWidth()-1)
			{				
				if(currentTile ==1 && nextTile ==1)
				{
					countX++;
					leftOverTiles[x+y*GridWidth]=0;
					if(!RectanglePosSet)
					{
						rectPosition = Vector2D(x*tileWidth, y*tileHeight);
						RectanglePosSet = true;
					}
				}
				else if(currentTile ==1 && nextTile ==0 && RectanglePosSet)
				{
					countX++;
					leftOverTiles[x+y*GridWidth]=0;
					rects.push_back(new CollisionRect(Vector2D(rectPosition.x+countX*tileWidth/2, rectPosition.y + tileHeight/2), countX*tileWidth, tileHeight));
					RectanglePosSet = false; 
					countX = 0;
				}
			}else
			{
				if(RectanglePosSet)
				{
					if(currentTile ==1)
					{
						countX++;
						leftOverTiles[x+y*tiles->getGridWidth()]=0;
						rects.push_back(new CollisionRect(Vector2D(rectPosition.x+countX*tileWidth/2, rectPosition.y + tileHeight/2), countX*tileWidth, tileHeight));
						RectanglePosSet = false; 
						countX = 0;
					}
				}
			}			
		}
	}
	//Verticle Rectangle creation.
	for(int x=0; x<tiles->getGridWidth();x++)
	{
		for(int y=0; y<tiles->getGridHeight();y++)
		{		
			
			currentTile = leftOverTiles[x+y*GridWidth];			
			nextTile = leftOverTiles[x+(y+1)*GridHeight];
			if(y<GridHeight-1)
			{	
				if(currentTile ==1 && nextTile ==1)
				{
					countY++;
					leftOverTiles[x+y*GridWidth]=0;
					if(!RectanglePosSet)
					{
						rectPosition = Vector2D(x*tileWidth, y*tileHeight);
						RectanglePosSet = true;
					}
				}
				else if(currentTile ==1 && nextTile ==0 && RectanglePosSet)
				{
					countY++;
					leftOverTiles[x+y*GridWidth]=0;
					rects.push_back(new CollisionRect(Vector2D(rectPosition.x + tileWidth/2, rectPosition.y + countY* tileHeight/2), tileWidth, countY*tileHeight));
					RectanglePosSet = false; 
					countY = 0;
				}
			}else
			{
				if(RectanglePosSet)
				{
					if(currentTile ==1)
					{
						countY++;
						leftOverTiles[x+y*GridWidth]=0;
						rects.push_back(new CollisionRect(Vector2D(rectPosition.x + tileWidth/2, rectPosition.y + countY* tileHeight/2), tileWidth, countY*tileHeight));
						RectanglePosSet = false; 
						countY = 0;
					}
				}
			}			
		}
	}
	for(int i=0; i<leftOverTiles.size(); i++)
	{
		if(leftOverTiles[i] == 1)
		{
			Vector2D rectPosition = Vector2D((i%GridWidth)*tileWidth, (i/GridHeight)*tileHeight);
			rects.push_back(new CollisionRect(Vector2D(rectPosition.x + tileWidth/2, rectPosition.y + tileHeight/2), tileWidth, tileHeight)); 
		}
	}
}



