	#pragma once
#include "SpriteBatch.h"
#include "InputManager.h"
#include "Map.h"
#include "TileEngine.h"
#include <iostream>

void Initialize();
void LoadContent();
void Update(float gameTime);
void Draw();

//Destroys the components of the game when the application is ending
void DestroyGame();

//Main Variables
int ScreenWidth;
int ScreenHeight;

//Textures/Font refrences
int BlazedFont;
int explosion;
int brick;
int floortile;
int background;

//Objects on screen
std::vector<PhysicsObject2D*> Players;
std::vector<PhysicsObject2D*> Bounds;


SDL_Rect backgroundSource;
SDL_Rect backgroundDest;


PhysicsEngine* engine;

PhysicsObject2D* thing;
//PhysicsObject2D* thing2;
PhysicsObject2D* thing3;


TileEngine* tileEngine;

Map* gameMap;

bool quit = false;
SpriteFont* font;
MapCellAnimated* cell;
SpriteBatch* spriteBatch;//The spritebatch to draw with
InputManager* input;//The input instance to poll
//PackedSprite test;

//DO NOT MODIFY MAIN
int main(int argv, char** argc)
{
	Initialize();
	LoadContent();
	
	
	
	for(int y =0; y<tileEngine->getGridHeight();y++)
	{
		for(int x =0; x<tileEngine->getGridWidth();x++)
		{
			if(tileEngine->levelVec[x+(y*tileEngine->getGridWidth())] ==1)
			{
				printf("1");

				Bounds.push_back(new PhysicsObject2D(Bounds.size(),Vector2D(25+50*x,25+50*y),CollisionRect(Vector2D(0.0,0.0),50,50)));
				//Toggle whether objects are static or not
				Bounds[Bounds.size() - 1]->movable = false;
				Bounds[Bounds.size() - 1]->SetMass(1.0f);
			}
			printf("0");
		}
		printf("\n");
	}

	for(int j =0; j<Bounds.size();j++)
	{
		gameMap->RegisterStaticObject(Bounds[j]);
	}


	backgroundSource = spriteBatch->QueryTexture(background);
	backgroundDest = spriteBatch->QueryTexture(background);
	

	float timeThisFrame = 0;
	float timeLastFrame = 0;
	float deltaTime = 0;

	//DO NOT MODIFY THIS METHOD
	do
	{
		//Set lastframe time to this frames time
		timeLastFrame = timeThisFrame;
		//Set this frame time to time since starting the program
		timeThisFrame = (float)SDL_GetTicks();
		//Get the number of seconds that have passed since last frame
		deltaTime = (timeThisFrame - timeLastFrame) / 1000;
		//Update the game
		Update(deltaTime);
		//Draw the game
		Draw();
	} while (!quit);

	DestroyGame();

	return 0;
}

//Initialize the game components
void Initialize()
{
	spriteBatch = SpriteBatch::GetInstance();
	input = InputManager::GetInstance();
	tileEngine = TileEngine::GetInstance("LevelMap2", false);
	engine = PhysicsEngine::GetInstance();

	PhysicsObject2D::setMapBounds(new Vector2D(ScreenWidth,ScreenHeight));
	cell = new MapCellAnimated(explosion,1,1,50,2048,1536,Vector2D(8,6),0.05f,45,true,spriteBatch);
	gameMap = new Map(20,20,50,floortile,spriteBatch);

	//Initialize the window
	ScreenWidth = tileEngine->getGridWidth()*tileEngine->getTileWidth();
	ScreenHeight = tileEngine->getGridHeight()*tileEngine->getTileHeight();
	spriteBatch->Initialize("Test",ScreenWidth ,ScreenHeight);
	
	//Any initialization here
}

//Performs any loading
void LoadContent()
{
	//Load contents here

	font = SpriteFont::GetInstance();
	BlazedFont = font->LoadFont("Blazed.ttf", 16);
	floortile = spriteBatch->LoadTexture(std::string("Assets//Tilesheets//") + tileEngine->getTileSheet() + std::string("//FloorTile.png"));
	background = spriteBatch->LoadTexture(std::string("Assets//Tilesheets//") + tileEngine->getTileSheet() + std::string("//Background.jpg"));
	explosion = spriteBatch->LoadTexture("Assets//Explosion1.png");

}

//Updates the game
void Update(float gameTime)
{
	engine->Update(gameTime);
	input->Update();
	cell->Update(gameTime);
	gameMap->Update(gameTime);
	
	//if escape is pressed, quit the game
	if(input->GetKeyPress(SDL_SCANCODE_ESCAPE))
	{
		quit = true;
	}
	
	for each (PhysicsObject2D* object in Bounds)
	{
		object->Update(gameTime);
	}
	
	//Game Updating goes under here
}

//Draws the game
void Draw()
{
	//Clear the renderer
	spriteBatch->ClearRenderer();
	
	//Call spritebatch begin and end. Draw in between them
	spriteBatch->Begin(SpriteBatchSortMode::Immediate,SDL_BLENDMODE_BLEND);
	spriteBatch->DrawTexture(background,CreateColor(0.5f,0.5f,0.5f,1.0f),&backgroundSource,&backgroundDest,0.0f,NULL,SDL_RendererFlip::SDL_FLIP_NONE,1.0f);
	gameMap->Draw(spriteBatch);
	

	for each (PhysicsObject2D* object in Bounds)
	{
		object->Draw(spriteBatch);
	}
	cell->Draw(spriteBatch);
	spriteBatch->End();
}

//Destroys the components of the game when the application is ending
void DestroyGame()
{
	if(spriteBatch)
		spriteBatch->DestroySpriteBatch(spriteBatch);

	if(input)
		input->DestroyInputManager(input);
}

int GetNewId()
{
}

void UpdateInput(int PlayerID,SDL_Scancode keypress)
{
	if(keypress == SDL_SCANCODE_W)
	{
		for each (PhysicsObject2D* object in Players)
		{
			if(object->id == PlayerID)
			{
				object->AddImpulse(Vector2D(0,-1.0),10);
			}
		}		
	}
	if(keypress == SDL_SCANCODE_S)
	{
		for each (PhysicsObject2D* object in Players)
		{
			if(object->id == PlayerID)
			{
				object->AddImpulse(Vector2D(0,1.0),10);
			}
		}	
	}
	if(keypress == SDL_SCANCODE_A)
	{
		for each (PhysicsObject2D* object in Players)
		{
			if(object->id == PlayerID)
			{
				object->AddImpulse(Vector2D(-1.0,0),10);
			}
		}	
	}
	if(keypress == SDL_SCANCODE_D)
	{
		for each (PhysicsObject2D* object in Players)
		{
			if(object->id == PlayerID)
			{
				object->AddImpulse(Vector2D(1.0,0),10);
			}
		}		
	}
}