#pragma once
#include "PhysicsEngine.h"
#include "MapCellAnimated.h"
class Map
{
public:	

	Map(int _width, int _height, float _cellSpacing,int TextureIndex,SpriteBatch* spriteBatch);
	Map(void);
	~Map(void);
	void RegisterObjectsOnGrid();
	void RegisterObject(PhysicsObject2D* _object);
	void RegisterStaticObject(PhysicsObject2D* _object);
	void RegisterActiveCells();
	void RegisterStaticObjectsOnGrid();
	void RegisterRectangleOnGrid(PhysicsObject2D* _object, bool staticObject);
	void Update(float gameTime);
	void Draw(SpriteBatch* spriteBatch);
	int CalculateGridPos(Vector2D* _position);
	
	//MapCell*[] GetGrid(); //I dont know if this would be used...
private:
	PhysicsEngine* engine;
	std::vector<MapCell*>ActiveCells;
	std::vector<PhysicsObject2D*>RegisteredPhysicsObjects;
	std::vector<PhysicsObject2D*>RegisteredStaticPhysicsObjects;
	int num;
	float cellSpacing;
	int width;
	int height;
	MapCell* grid;
	int clamp(int value, int lower, int upper){return std::max(lower,std::min(upper,value));}


};

