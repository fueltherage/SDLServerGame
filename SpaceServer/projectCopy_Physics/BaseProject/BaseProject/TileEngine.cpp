#include "TileEngine.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

static TileEngine* instance;

TileEngine::TileEngine(std::string _fileToRead, bool _isServer){
	isServer = _isServer;
	levelVec = ProcessFile(_fileToRead);
}

TileEngine* TileEngine::GetInstance(std::string _fileToRead, bool _isServer){
	if (instance == nullptr)instance = new TileEngine(_fileToRead, _isServer);
	return instance;
}

/*Reads a specified text file and parses it.
param:_fileToRead->Name of file to parse. We will be using the .alevel format.
*/
std::vector<int> TileEngine::ProcessFile(std::string _fileToRead){
	//Create file
	std::ifstream myFile;
	//Create stringstream that will hold onto string and feed them into appropriate variables
	std::stringstream ss;
	//Create string that will hold the tile characters once read
	std::string strValues;
	//Opening file
	myFile.open(_fileToRead + ".aLevel", std::ios::out | std::ios::app);
	std::string currentLineState;
	if (myFile.is_open()){
		//Read file line by line until it reaches end of file
		std::string tempLine;
		while (!myFile.eof()){
			tempLine = "";
			std::getline(myFile, tempLine);
			if (tempLine == "--MAIN--" || tempLine == "--SETTINGS--" || tempLine == "--PASSABLE--" || tempLine == "--TILESHEET--"){
				currentLineState = tempLine;
			} else if (currentLineState == "--MAIN--"){
				strValues += tempLine;
			} else if (currentLineState == "--SETTINGS--"){
				ss.clear();
				ss.str("");
				ss << tempLine;
				ss >> gridWidth >> gridHeight >> tileWidth >> tileHeight;
			} else if (currentLineState == "--PASSABLE--"){
				ss.clear();
				ss.str("");
				ss << tempLine;
				int passableNum;
				while (ss >> passableNum) passableTilesVec.push_back(passableNum);
			} else if (currentLineState == "--TILESHEET--" && !isServer){ //server doesn't need to read tilesheet information
				tileSheet = tempLine;
			}
		}
	}
	ss.clear();
	ss.str("");
	ss << strValues;
	std::vector<int> vecValues;
	int segment;
	while (ss >> segment){
		vecValues.push_back(segment);
	}

	return vecValues;
}

int TileEngine::getGridWidth(){
	return gridWidth;
}

int TileEngine::getGridHeight(){
	return gridHeight;
}

int TileEngine::getTileWidth(){
	return tileWidth;
}

int TileEngine::getTileHeight(){
	return tileHeight;
}

std::string TileEngine::getTileSheet(){
	return tileSheet;
}

/*Returns the selected index of the tile array as a point along a 2D grid.
The first point starts at (0,0) + half tile width and height and goes in a
sotheast direction.
param:_tileIndex->The specified number index of the tile array
*/
SDL_Point TileEngine::ConvertToPoint(int _tileIndex){
	SDL_Point point = SDL_Point();
	point.x = (_tileIndex - (gridWidth * ::ceil(_tileIndex / gridWidth)));
	point.y = ::ceil(_tileIndex / gridWidth);
	//point.x = point.x * tileWidth + (tileWidth / 2);
	//point.y = point.y * tileHeight + (tileHeight / 2);
	//return point;
	return GridToWorld(point);
}

/*Returns the index form of a point on the 2D gird.
param:_point->The specified point on the 2D grid*/
int TileEngine::ConvertToIndex(SDL_Point _point){
	//Given that the point stays in the same spot as it did when it was generated. We'll have to change it later
	//int x = 0, y = 0;
	int indexToReturn = 0;
	//x = (_point.x - (tileWidth / 2)) / tileWidth;
	//y = (_point.y - (tileHeight / 2)) / tileHeight;
	SDL_Point point = WorldToGrid(_point);
	indexToReturn = (point.x + (point.y * gridWidth));
	return indexToReturn;
}

//Converts a Point from world space to grid space
SDL_Point TileEngine::WorldToGrid(SDL_Point _point) {
	SDL_Point gridPoint = SDL_Point();
	gridPoint.x = _point.x / tileWidth;//Integer division will automatically round it down
	gridPoint.y = _point.y / tileHeight;
	return gridPoint;
}

//Converts a Point from grid space to world space
//Returns the world coordinate in the center of the tile
SDL_Point TileEngine::GridToWorld(SDL_Point _point) {
	SDL_Point worldPoint = SDL_Point();
	worldPoint.x = _point.x * tileWidth + tileWidth/2;
	worldPoint.y = _point.y * tileHeight + tileHeight/2;
	return worldPoint;
}

//Checks if the tile at the specified index is passable
//Returns 0 if false, 1 if true
int TileEngine::IsPassable(int _tileIndex) {
	for(int i = 0; i<passableTilesVec.size(); i++) {
		if(passableTilesVec[i] == levelVec[_tileIndex]) {
			return 0;
		}
	}
	return 1;
}

//Checks if a point in world space is on a passable tile or not
//Returns 0 if false, 1 if true
int TileEngine::CheckCollision(SDL_Point _worldPoint) {
	return IsPassable(ConvertToIndex(_worldPoint));
}

void TileEngine::Update(){

}

//void TileEngine::Draw(SpriteBatch* _spriteBatch, MapCell* _cellsToDraw, int _amountOfCells){
//	for (int i = 0; i<_amountOfCells; i++){
//		_cellsToDraw[i].Draw(_spriteBatch);
//	}
//}