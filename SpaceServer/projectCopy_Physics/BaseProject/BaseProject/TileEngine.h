#include <stdio.h>
#include <SDL.h>
#include <string>
#include <vector>
//#include "MapCell.h"
class TileEngine
{
public:
	std::vector<int> levelVec;
	std::vector<int> passableTilesVec;
	static TileEngine* GetInstance(std::string _fileToRead, bool _isServer);
	std::vector<int> ProcessFile(std::string _fileToRead);
	void Update();
	//void Draw(SpriteBatch* _spriteBatch, MapCell* _cellsToDraw, int _amountOfCells);
	int getGridWidth();
	int getGridHeight();
	int getTileWidth();
	int getTileHeight();
	std::string getTileSheet();
	SDL_Point ConvertToPoint(int _tileIndex);
	int ConvertToIndex(SDL_Point _point);
	SDL_Point WorldToGrid(SDL_Point _point);
	SDL_Point GridToWorld(SDL_Point _point);
	int IsPassable(int _tileIndex);
	int CheckCollision(SDL_Point _worldPoint);
private:
	int tileWidth, tileHeight, gridWidth, gridHeight;
	std::string tileSheet;
	bool isServer;
	TileEngine(std::string _fileToRead, bool _isServer);
	~TileEngine(void);
};
