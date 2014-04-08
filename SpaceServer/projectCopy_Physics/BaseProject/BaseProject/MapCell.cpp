#include "MapCell.h"
//Notes
/*
	Each list object must have a unique id value. Duplicates are NOT ALLOWED. 

	Even duplicate objects need to have their own unique id
*/
MapCell::MapCell(void)
{
	x=0;
	y=0;
}
MapCell::~MapCell(void)
{

}
MapCell::MapCell(int TextureIndex,int _x, int _y, float gridSpacing, SpriteBatch* spriteBatch){
	Texture = TextureIndex;
	x = _x;
	y = _y;	
	cellDestRect.x = _x * (int)gridSpacing;
	cellDestRect.y = _y * (int)gridSpacing;
	cellDestRect.w = (int)gridSpacing;
	cellDestRect.h = (int)gridSpacing;

	//cellDestRect.w = frameWidth;
	//cellDestRect.h = frameHeight;

	cellSourceRect.x = 0;
	cellSourceRect.y = 0;
	cellSourceRect.w = spriteBatch->QueryTexture(Texture).w;
	cellSourceRect.h = spriteBatch->QueryTexture(Texture).h;
	position = Vector2D(x*gridSpacing, y*gridSpacing);
}
void MapCell::Add(PhysicsObject2D* _object)
{
	for(unsigned int i = 0; i<RegisteredObjects.size(); i++)
	{
		if(RegisteredObjects[i]->GetID() == _object->GetID())
		{
		return;
		//Throw error, cannot add the an object twice to a list
		}
	}
	RegisteredObjects.push_back(_object);
}
void MapCell::AddStaticObject(PhysicsObject2D* _object)
{
	for(unsigned int i = 0; i<RegisteredObjects.size(); i++)
	{
		if(RegisteredObjects[i]->GetID() == _object->GetID())
		{
		return;
		//Throw error, cannot add the an object twice to a list
		}
	}
	RegisteredStaticObjects.push_back(_object);
}
void MapCell::Remove(PhysicsObject2D* _object)
{
	for(unsigned int i = 0; i<RegisteredObjects.size(); i++)
	{
		if(RegisteredObjects[i]->GetID() == _object->GetID())
		{
			RegisteredObjects.erase(RegisteredObjects.begin()+i);
			return;
		}
	}	
}
bool MapCell::Has(PhysicsObject2D* _object)
{
	for(unsigned int i = 0; i<RegisteredObjects.size() ;i++)
	{
		if(RegisteredObjects[i]->GetID() == _object->GetID())
		{			
			return true;
		}
	}
	return false;
}
void MapCell::Update(float gameTime)
{
		
}
void MapCell::Draw(SpriteBatch* spriteBatch){
	if(Active()){
	
		int count = Count();
		//printf("\n red: %i, green: %i, blue: %i, alpha: %i",count*10,255-count*10,0,255);
		spriteBatch->DrawTexture(Texture,CreateColor((int)clamp(count*35,0,255),(int)clamp(255-count*35,0,255),0,255),&cellSourceRect,&cellDestRect,0.0f,NULL,SDL_RendererFlip::SDL_FLIP_NONE,0.5f);
	}
	else{spriteBatch->DrawTexture(Texture,CreateColor(125,124,125,255),&cellSourceRect,&cellDestRect,0.0f,NULL,SDL_RendererFlip::SDL_FLIP_NONE,0.5f);
	}
}

//Clamps a value between and upper and lower range
int clamp(int value, int lower, int upper)
{
	return std::max(upper,std::min(lower,value));
}