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
std::vector<PhysicsObject2D*> things;
std::vector<PhysicsObject2D*> movable;
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
	font = SpriteFont::GetInstance();
	BlazedFont = font->LoadFont("Blazed.ttf", 16);
	floortile = spriteBatch->LoadTexture(std::string("Assets//Tilesheets//") + tileEngine->getTileSheet() + std::string("//FloorTile.png"));
	background = spriteBatch->LoadTexture(std::string("Assets//Tilesheets//") + tileEngine->getTileSheet() + std::string("//Background.jpg"));
	explosion = spriteBatch->LoadTexture("Assets//Explosion1.png");
	
	PhysicsObject2D::setMapBounds(new Vector2D(ScreenWidth,ScreenHeight));
	cell = new MapCellAnimated(explosion,1,1,50,2048,1536,Vector2D(8,6),0.05f,45,true,spriteBatch);
	gameMap = new Map(20,20,50,floortile,spriteBatch);
	//Cluster of 100 objects

	
	for(int y =0; y<tileEngine->getGridHeight();y++)
	{
		for(int x =0; x<tileEngine->getGridWidth();x++)
		{
			if(tileEngine->levelVec[x+(y*tileEngine->getGridWidth())] ==1)
			{
				printf("1");

				things.push_back(new PhysicsObject2D(things.size(),Vector2D(25+50*x,25+50*y),CollisionRect(Vector2D(0.0,0.0),50,50)));
				//Toggle whether objects are static or not
				things[things.size() - 1]->movable = false;
				things[things.size() - 1]->SetMass(1.0f);
			}
			printf("0");
		}
		printf("\n");
	}


	for(int j =0; j<things.size();j++)
	{
		gameMap->RegisterStaticObject(things[j]);
	}
	//thing2 = new PhysicsObject2D(things.size(), Vector2D(300,300), CollisionRect(Vector2D(0,0),10,10));
	//thing2->SetDrag(0.25f);
	//thing2->SetMass(1.0f);
	//things.push_back(thing2);

	for(int i=0; i<20;i++)
	{
		thing3 = new PhysicsObject2D(things.size(), Vector2D(30*i+100,100), CollisionRect(Vector2D(0,0),20,20));
		thing3->SetDrag(0.75f);
		thing3->SetMass(2.0f);
		things.push_back(thing3);
		movable.push_back(thing3);
		gameMap->RegisterObject(thing3);
	}
	//gameMap->RegisterObject(thing2);
	//gameMap->RegisterObject(thing3);

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
	
	for each (PhysicsObject2D* object in things)
	{
		object->Update(gameTime);
	}



	//thing2 will follow the mouse around
	SDL_Point MousePosition = input->GetMousePosition();
	//Converting to Vector2D format for Wes's pleasure
	Vector2D MouseDirection = Vector2D(MousePosition.x,MousePosition.y);
	//thing2->SpringForce(MouseDirection,0,1000000.0f);

	if(input->GetKeyDown(SDL_SCANCODE_W))
	{
		for each (PhysicsObject2D* object in movable)
		{
			object->AddImpulse(Vector2D(0,-1.0),10);
		}
		//thing3->AddForce(Vector2D(0,-1.0),100);
	}
	if(input->GetKeyDown(SDL_SCANCODE_S))
	{
		for each (PhysicsObject2D* object in movable)
		{
			object->AddImpulse(Vector2D(0,1.0),10);
		}
		//thing3->AddForce(Vector2D(0,1.0),100);
	}
	if(input->GetKeyDown(SDL_SCANCODE_A))
	{
		for each (PhysicsObject2D* object in movable)
		{
			object->AddImpulse(Vector2D(-1,0),10);
		}
		//thing3->AddForce(Vector2D(-1.0,0),100);
	}
	if(input->GetKeyDown(SDL_SCANCODE_D))
	{
		for each (PhysicsObject2D* object in movable)
		{
			object->AddImpulse(Vector2D(1,0),10);
		}
		//thing3->AddForce(Vector2D(1.0,0),100);
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
	

	for each (PhysicsObject2D* object in things)
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