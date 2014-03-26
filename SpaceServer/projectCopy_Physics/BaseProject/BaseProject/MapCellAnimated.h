#pragma once
#include "mapcell.h"
class MapCellAnimated :
	public MapCell
{
public:	
	//_x _y are the index of the MapCell in Map
	//_gridSpacing - is the width and height of MapCells
	//_frameWidth _frameHeight controls with width and height of the animation frame
	//SheetSetup - (x,y) x - how many frames wide  y - how many frames height
	//_timeBetweenFrames - controls framerate of animation
	//Looping - loops the animation forever
	MapCellAnimated(int _Texture,int _x, int _y, float _gridSpacing, float _frameWidth, float _frameHeight,Vector2D SheetSetup, float _timeBetweenFrames,int _numberOfFrames, bool looping,SpriteBatch* spriteBatch);
	MapCellAnimated(void);
	~MapCellAnimated(void);
	void NextFrame();
	void Update(float gameTime);
	
private:
	float timeBetweenFrames;
	float sheetWidth;
	float sheetHeight;
	float frameWidth;
	float frameHeight;
	float elapsedTime;
	int numberOfFrames;
	int FramesWide;
	int FramesHeigh;
	int currentFrame;
	bool looping;
};

