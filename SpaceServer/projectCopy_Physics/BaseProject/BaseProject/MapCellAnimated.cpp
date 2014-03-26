#include "MapCellAnimated.h"
#include "iostream"

//Notes
//Known bugs:
/*
	Bug 1: When the grid is over 17 tiles in size it makes the 18th tile null
	this crashes the program
	
	Bug 2: the first element in grid[] for some reason makes cellDestRect's x y values go
	way off screne

	This is very strange because grid[] values dont get altered at all
*/

	
MapCellAnimated::MapCellAnimated(int _Texture,int _x, int _y, float _gridSpacing, float _sheetWidth, float _sheetHeight,Vector2D SheetSetup, float _timeBetweenFrames,int _numberOfFrames, bool _looping,SpriteBatch* spriteBatch):MapCell(_Texture,_x,_y,_gridSpacing,spriteBatch)
{
	sheetWidth = _sheetWidth;
	sheetHeight = _sheetHeight;
	FramesWide = (int)SheetSetup.x;
	FramesHeigh = (int)SheetSetup.y;
	numberOfFrames = _numberOfFrames;
	timeBetweenFrames  = _timeBetweenFrames;
	frameWidth = sheetWidth/(float)FramesWide;
	frameHeight = sheetHeight/(float)FramesHeigh;

	cellDestRect.x = _x * (int)_gridSpacing;
	cellDestRect.y = _y * (int)_gridSpacing;
	cellDestRect.w = (int)_gridSpacing;
	cellDestRect.h = (int)_gridSpacing;

	cellSourceRect.x = 0;
	cellSourceRect.y = 0;
	cellSourceRect.w = (int)sheetWidth/FramesWide;
	cellSourceRect.h = (int)sheetHeight/FramesHeigh;


	looping = _looping;
	elapsedTime =0;
	Texture = _Texture;
	currentFrame =0;
}

MapCellAnimated::MapCellAnimated(void)
{
}


MapCellAnimated::~MapCellAnimated(void)
{
}
void MapCellAnimated::Update(float gameTime) 
{
	elapsedTime += gameTime;

	if(elapsedTime >= timeBetweenFrames)
	{
		NextFrame();
		elapsedTime =0;
	}
	MapCell::Update(gameTime);
}
void MapCellAnimated::NextFrame()
{
	//printf("currentFrame %i\n",currentFrame);
	currentFrame++;
	if(currentFrame >= numberOfFrames)
	{
		currentFrame = 0;
	}
	cellSourceRect.x = (currentFrame%FramesWide)*(int)frameWidth;
	cellSourceRect.y = (currentFrame/FramesWide)*(int)frameHeight;
}
