#include "Map.h"
#include "SpriteBatch.h"


Map::Map(int _width, int _height, float _cellSpacing,int TextureIndex,SpriteBatch* spriteBatch)
{
	width = _width;
	height = _height;
	cellSpacing = _cellSpacing;
	//grid = new MapCellAnimated[width*height]();
	grid = new MapCell[width*height]();

	for(int w=0; w<width; w++)
	{
		for(int h=0; h<height; h++)
		{
			num = (w*height)+h;
			//grid[num] = MapCellAnimated(TextureIndex,w,h,cellSpacing,2048,1536,Vector2D(8,6),0.05f,45,true);
			grid[num] = MapCell(TextureIndex,w,h,cellSpacing,spriteBatch);
		}
	}
	engine = PhysicsEngine::GetInstance();
}
Map::Map(void)
{
	engine = PhysicsEngine::GetInstance();
}
Map::~Map(void)
{
}
void Map::RegisterActiveCells()
{
	ActiveCells.clear();
	for(int w=0; w<width; w++)
	{
		for(int h=0; h<height; h++)
		{
			num = (w*height)+h;
			if(grid[num].Active())
			{
				ActiveCells.push_back(&grid[num]);
			}
		}
	}
}
void Map::RegisterObject(PhysicsObject2D* _object)
{
	for(unsigned int i=0;i<RegisteredPhysicsObjects.size();i++)
	{
		if(RegisteredPhysicsObjects[i]->GetID() == _object->GetID())
		{
			return;
		}		
	}
	RegisteredPhysicsObjects.push_back(_object);
	RegisterRectangleOnGrid(_object,false);
}
void Map::RegisterStaticObject(PhysicsObject2D* _object)
{
	//if the object is already registered then dont register it
	for(unsigned int i=0; i<RegisteredStaticPhysicsObjects.size(); i++)
	{
		if(RegisteredStaticPhysicsObjects[i]->GetID() == _object->GetID())
		{
			return;
		}		
	}
	RegisteredStaticPhysicsObjects.push_back(_object);

	RegisterRectangleOnGrid(_object,true);
}
void Map::RegisterObjectsOnGrid()
{
	//Remove all references to objects
	if(RegisteredPhysicsObjects.size() != 0)
	{
		for(int i=0; i< width*height; i++)
		{
			for(unsigned int s=0; s<RegisteredPhysicsObjects.size(); s++)
			{
				grid[i].Remove(RegisteredPhysicsObjects[s]);
			}
		}
	}
	//Re-add the references
	for(unsigned int s=0; s<RegisteredPhysicsObjects.size(); s++)
	{
		RegisterRectangleOnGrid(RegisteredPhysicsObjects[s],false);
	}
}
void Map::RegisterStaticObjectsOnGrid()
{
	//Remove all references to static objects
	for(int i=0; i< width*height; i++)
	{
		for(unsigned int s=0; s<RegisteredStaticPhysicsObjects.size(); s++)
		{
			grid[i].Remove(RegisteredStaticPhysicsObjects[s]);
		}
	}
	//Re-add the references
	for(unsigned int s=0; s<RegisteredStaticPhysicsObjects.size(); s++)
	{
		RegisterRectangleOnGrid(RegisteredStaticPhysicsObjects[s],true);
	}
}
void Map::RegisterRectangleOnGrid(PhysicsObject2D* _object, bool staticObject)
{
	Vector2D c1World = _object->collider.c1World();
	Vector2D c2World = _object->collider.c2World();
	Vector2D c3World = _object->collider.c3World();
	Vector2D c4World = _object->collider.c4World();

	int c1GridPos = CalculateGridPos(&c1World);
	int c2GridPos = CalculateGridPos(&c2World);
	int c3GridPos = CalculateGridPos(&c3World);
	int c4GridPos = CalculateGridPos(&c4World);

	bool c1c2 =  (c1GridPos == c2GridPos);
	bool c1c3 =  (c1GridPos == c3GridPos);
	bool c1c4 =  (c1GridPos == c4GridPos);

	
	if(c1c2 && c1c3 && c1c3) 
	{
		if(staticObject)
		{
			grid[clamp(c1GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos+height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos+1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos+height-1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos+height+1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-height-1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-height+1,0,(width*height)-1)].AddStaticObject(_object);
		}else {
			grid[clamp(c1GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos+height,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-height,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos+1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos+height-1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos+height+1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-height-1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-height+1,0,(width*height)-1)].Add(_object);
		}
	}
	if(!c1c2 && !c1c3 && !c1c3)
	{
		if(staticObject)
		{
			grid[clamp(c1GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c2GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c3GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c4GridPos,0,(width*height)-1)].AddStaticObject(_object);
		}else 
		{
			grid[clamp(c1GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c2GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c3GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c4GridPos,0,(width*height)-1)].Add(_object);
		}
	}
	if(c1c2 && !c1c4)
	{
		if(staticObject)
		{
			grid[clamp(c1GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos+height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-1+height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-1-height,0,(width*height)-1)].AddStaticObject(_object);

			grid[clamp(c3GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c3GridPos+height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c3GridPos-height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c3GridPos+1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c3GridPos+1+height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c3GridPos+1-height,0,(width*height)-1)].AddStaticObject(_object);	
		}
		else {
			grid[clamp(c1GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos+height,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-height,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-1+height,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-1-height,0,(width*height)-1)].Add(_object);

			grid[clamp(c3GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c3GridPos+height,0,(width*height)-1)].Add(_object);
			grid[clamp(c3GridPos-height,0,(width*height)-1)].Add(_object);
			grid[clamp(c3GridPos+1,0,(width*height)-1)].Add(_object);
			grid[clamp(c3GridPos+1+height,0,(width*height)-1)].Add(_object);
			grid[clamp(c3GridPos+1-height,0,(width*height)-1)].Add(_object);	
		}
	}
	if(c1c4 && !c1c2)
	{
		if(staticObject)
		{
			grid[clamp(c1GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos+1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-height+1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c1GridPos-height-1,0,(width*height)-1)].AddStaticObject(_object);

			grid[clamp(c2GridPos,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c2GridPos+1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c2GridPos-1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c2GridPos+height,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c2GridPos+height+1,0,(width*height)-1)].AddStaticObject(_object);
			grid[clamp(c2GridPos+height-1,0,(width*height)-1)].AddStaticObject(_object);
		}else{
			grid[clamp(c1GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos+1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-height,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-height+1,0,(width*height)-1)].Add(_object);
			grid[clamp(c1GridPos-height-1,0,(width*height)-1)].Add(_object);

			grid[clamp(c2GridPos,0,(width*height)-1)].Add(_object);
			grid[clamp(c2GridPos+1,0,(width*height)-1)].Add(_object);
			grid[clamp(c2GridPos-1,0,(width*height)-1)].Add(_object);
			grid[clamp(c2GridPos+height,0,(width*height)-1)].Add(_object);
			grid[clamp(c2GridPos+height+1,0,(width*height)-1)].Add(_object);
			grid[clamp(c2GridPos+height-1,0,(width*height)-1)].Add(_object);
		}
	}
}

int Map::CalculateGridPos(Vector2D* _position)
{	
	int x = (int)(_position->x / cellSpacing);
	int y = (int)(_position->y / cellSpacing);
	return (x * height) + y;
}
void Map::Update(float gameTime)
{
	RegisterActiveCells();
	for(int i = 0; i<ActiveCells.size();i++)
	{
		//This currently loops through all active cells checking every registeredObject in those cells 
		for(int  z = 0; z < ActiveCells[i]->RegisteredObjects.size(); z++)
		{
			for(int s = z + 1; s < ActiveCells[i]->RegisteredObjects.size(); s++)
			{	
				engine->RegisterPairedCollision(ActiveCells[i]->RegisteredObjects[z], ActiveCells[i]->RegisteredObjects[s]); 
			}
			for(int c = 0; c < ActiveCells[i]->RegisteredStaticObjects.size(); c++)
			{	
				engine->RegisterPairedCollision(ActiveCells[i]->RegisteredObjects[z], ActiveCells[i]->RegisteredStaticObjects[c]); 
			}
		}		 
	}

	for(int i = 0; i<width*height;i++)
	{
		grid[i].MapCell::Update(gameTime);
	}
	RegisterObjectsOnGrid();
	
	
}
void Map::Draw(SpriteBatch* spriteBatch)
{
	for(int i=0; i<width*height; i++)
	{
		grid[i].Draw(spriteBatch);
				
	}
}

