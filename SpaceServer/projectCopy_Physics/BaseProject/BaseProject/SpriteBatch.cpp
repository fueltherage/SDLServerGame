#pragma once
#include "SpriteBatch.h"
#include <iostream>

#ifndef COLOR_WHITE
#define COLOR_WHITE CreateColor(255, 255, 255, 255)
#endif

bool operator== (const SDL_Color c1, const SDL_Color c2)
{
	return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b) && (c1.a == c2.a);
};

bool operator!= (const SDL_Color c1, const SDL_Color c2)
{
	return (c1.r != c2.r) || (c1.g != c2.g) || (c1.b != c2.b) || (c1.a != c2.a);
};

//The instance to provide
static SpriteBatch* instance = nullptr;

//Constructor. Initializes SDL and SDL image
SpriteBatch::SpriteBatch(void)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
	}

	//For some reason, 0 is the error returned on this function, while all others are -1. So check for it
	if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
	{
		std::cout<<"SDL_IMAGE_Init Error: Can't find JPG or PNG DLL" << SDL_GetError() << std::endl;
	}

	//Initialize text rendering
	if(TTF_Init() != 0)
	{
		std::cout<<"SDL_TTF_INIT Error: Can't display text to screen. Check for proper DLL set-up"<<std::endl;
	}

	///Initialize renderer and window to null ptr to prevent uninitialized access
	renderer = nullptr;
	window = nullptr;

	//nullptr to the array of maps
	sortedImages = nullptr;

	//Initialize draw stats
	begun = false;
	sortMode = SpriteBatchSortMode::Immediate;
	blendMode = SDL_BLENDMODE_BLEND;

	///Reserve 20 slots for the texture list
	textureList.reserve(20);

	///Reserve 5 slots for the surface list (most likely using textures more, so save the space)
	surfaceList.reserve(5);

	//Get the instance of the sprite fonts
	fontManager = SpriteFont::GetInstance();

	///Initialize array of multimaps
	sortedImages = new std::multimap<float, PackedSprite>[10];
}

//Deletes all allocated memory
SpriteBatch::~SpriteBatch(void)
{
	if(window)		
		SDL_DestroyWindow(window);

	if(renderer)
		SDL_DestroyRenderer(renderer);

	//if we have loaded a texture
	if(textureList.size() > 0)
	{
		while (textureList.size() > 0)
		{
			SDL_DestroyTexture(textureList.back());
			
			if(textureList.back() == NULL)
				printf("YAY");
			//delete textureList.back();
			textureList.pop_back();
		}	
	}
		//if we have loaded a texture
	if(surfaceList.size() > 0)
	{
		while (surfaceList.size() > 0)
		{
			SDL_FreeSurface(surfaceList.back());
			surfaceList.pop_back();
		}
	}

	///Delete our array of maps used for sorting the images to draw
	if(sortedImages)
	{
		delete[] sortedImages;
	}

	if(fontManager)
	{
		fontManager->DestroySpriteFont(fontManager);
	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

//Destroy's the instance of spritebatch as well as the pointer to it.
void SpriteBatch::DestroySpriteBatch(SpriteBatch* &spriteBatch)
{
	spriteBatch = nullptr;

	if(instance)
		delete instance;
}

//Gets the instance of the current sprite batch
SpriteBatch* SpriteBatch::GetInstance()
{
	if(instance == nullptr)
		instance = new SpriteBatch();

	return instance;
}

//Initializes a window with the specified title and in the particular position, size, and with
//The attributes created from the SDL_Flags:
//SDL_WINDOW_FULLSCREEN 
//fullscreen window
//SDL_WINDOW_FULLSCREEN_DESKTOP 
//fullscreen window at the current desktop resolution
//SDL_WINDOW_OPENGL 
//window usable with OpenGL context
//SDL_WINDOW_HIDDEN 
//window is not visible
//SDL_WINDOW_BORDERLESS 
//no window decoration
//SDL_WINDOW_RESIZABLE 
//window can be resized
//SDL_WINDOW_MINIMIZED 
//window is minimized
//SDL_WINDOW_MAXIMIZED 
//window is maximized
//SDL_WINDOW_INPUT_GRABBED 
//window has grabbed input focus
//SDL_WINDOW_ALLOW_HIGHDPI 
//window should be created in high-DPI mode if supported (>= SDL 2.0.1) 
int SpriteBatch::Initialize(std::string title, int x, int y, int w, int h, Uint32 sdlFlag)
{
	window = SDL_CreateWindow(title.c_str(), x, y, w, h, sdlFlag);

	//Error check the window
	if(window == NULL)
	{
		std::cout<<"Error Creating Window" << std::endl;
		return 1;
	}

	//Create renderer and use Accelerated flag (the 0)
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Error check the renderer
	if(renderer == NULL)
	{
		std::cout<<"Error Creating Renderer" << std::endl;
		return 1;
	}

	return 0;
}

//Initializes a window with the specified title and in the particular position & size
int SpriteBatch::Initialize(std::string title, int x, int y, int w, int h)
{
	return Initialize(title, x, y, w, h, SDL_WINDOW_OPENGL);
}

//Initializes a window with the specified title and at the specified size
int SpriteBatch::Initialize(std::string title, int w, int h)
{
	return Initialize(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL);
}

//Creates a window with the specified title. 
//If fullscreen is true, sets window mode to fullscreen, else
//Creates a window that is 100 pixels less than the desktop screen
int SpriteBatch::Initialize(std::string title, bool fullScreen)
{
	//if full screen, draw with full screen
	if(fullScreen)
		return Initialize(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_FULLSCREEN);
	else
	{
		//otherwise draw a square 100 pixels less than the monitor all the way around
		SDL_Rect size;
		SDL_GetDisplayBounds(0, &size);
		return Initialize(title, 100, 100, size.w, size.h, SDL_WINDOW_OPENGL);
	}
}

//Sets the blend mode to use on points, lines, and rectangles
//param:blendMode->The blendmode to apply
int SpriteBatch::SetPrimitiveBlendMode(SDL_BlendMode blendMode)
{
	return SDL_SetRenderDrawBlendMode(renderer, blendMode);
}

//Returns a texture from the passed file name. Requires Extension in the string
//Returns -1 if errors, otherwise will return 0 or greater for success. This
//successful int will be used to draw the textures later
int SpriteBatch::LoadTexture(std::string fileName)
{
	//Load with the surface
	SDL_Surface* surface = IMG_Load(fileName.c_str());

	//if it's null, throw an error
	if(surface == NULL)
	{
		std::cout<< "Error loading image" << std::endl;
		return -1;
	}
	else
	{
		//Create the texture from the surface
		SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
	
		//Free the surface
		SDL_FreeSurface(surface);
		
		if(tex != NULL)
		{
			//Add the texture to the list
			textureList.push_back(tex);
			return textureList.size() - 1;
		}
	}
	return -1;
}

//Loads an SDL Surface. Use Texture unless you know what you are doing
int SpriteBatch::LoadSurface(std::string fileName)
{
	SDL_Surface* returnSurface = IMG_Load(fileName.c_str());

	if(returnSurface == NULL)
	{
		std::cout<< "Error loading image" << std::endl;
		return -1;
	}

	surfaceList.push_back(returnSurface);
	return surfaceList.size() - 1;
}

//Clear the renderer. Returns 0 if successful, -1 if not
int SpriteBatch::ClearRenderer()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	if(SDL_RenderClear(renderer) == -1)
	{
		std::cout<<"Error clearing renderer" << std::endl;
		return -1;
	}

	return 0;
}

//Prepares the Spritebatch for drawing
//param spriteSorting->The sort mode to use for drawing
//param blendMode->The way to blend the drawn textures
//returns 0 if successful, -1 if not
int SpriteBatch::Begin(SpriteBatchSortMode spriteSorting, SDL_BlendMode blendMode)
{
	//If end hasn't been called, throw an error
	if(begun)
	{
		std::cout<<"Begin has already been called. Call End before calling Begin again"<<std::endl;
		return -1;
	}
	else
	{
		//Otherwise, prepare the spritebatch
		begun = true;
		sortMode = spriteSorting;
		this->blendMode = blendMode;
		return 0;
	}
}

//Draws the specified texture
//param:texture->the index given from load texture
//param:color->the tint to apply to the texture
//param:sourceRect->the rectangle to pull a smaller image from the texture with. NULL for whole texture
//param:destRect->The rectangle to draw to. NULL to draw to whole renderer (screen)
//param:angle->The angle to rotate the image
//param:origin->The origin to rotate around. If Null, defaults to destRect center
//param:flip->SDL_RendererFlip::None/FlipHorizontal/FlipVertical. If you want both Flip Horizontal and Flip Vertical,
//use the bitwise operator '|'
//param:layerDepth->The depth to draw the image at
//Returns -1 on error, returns 0 on success
int SpriteBatch::DrawTexture(Uint32 texture, SDL_Color color, const SDL_Rect* sourceRect, const SDL_Rect* destRect,
							 float angle, const SDL_Point* origin, SDL_RendererFlip flip,  float layerDepth )
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}
	//make sure we have a valid layer depth
	if(layerDepth < 0 || layerDepth > 1)
	{
		std::cout <<"Layer Depth must be between 0 and 1"<<std::endl;
		return -1;
	}

	//Make sure we have a valid renderer
	if(!renderer)
	{	
		std::cout<<"Renderer is null. Did you Initialize Spritebatch?"<<std::endl;
		return -1;	
	}
	try{
		//Get the texture (at performs bounds checking, which will validate the index for us, regardless of sort mode)
		SDL_Texture* tex = textureList.at(texture);
		
		//If sort mode is immediate, draw right away
		if(sortMode == SpriteBatchSortMode::Immediate)
		{
			//Apply any blends
			ApplyBlendToTexture(tex, color);
			
			//if no flip or angle, draw with Render Copy
			if(angle == 0 && flip == 0)
				SDL_RenderCopy(renderer, tex, sourceRect, destRect);
			else
				///Draw the image with any flip/angle
				SDL_RenderCopyEx(renderer, tex, sourceRect, destRect, angle, origin, flip);
		}
		else
		{
			//Take the layer depth, multiply by 10 and floor it. A layer depth of 0.09 would then go into
			//index 0 map, while a 0.99 will go into index 9
			int sortIndex = (int)std::floor(layerDepth * 10);

			//Package the information
			PackedSprite package = PackedSprite(PackedSprite::PackType::Sprite, texture, sourceRect, 
				destRect, origin, color, flip, angle);
			//if sort index is greater than 9, put it in array slot 9. We do this because 
			//1.0 * 10 = floor(10) = 10. Our array only goes to the 9th index
			if(sortIndex >= 9)
			{
				sortedImages[9].insert(std::pair<float, PackedSprite>(layerDepth, package));
			}
			else
			{
				//otherwise, just put it in the appropriate slot
				sortedImages[sortIndex].insert(std::pair<float, PackedSprite>(layerDepth, package));
			}
		}
	}
	//If invalid index, we throw the error
	catch (std::out_of_range problem)
	{
		std::cout<<"SPRITEBATCH_DRAW ERROR: Texture not a valid index"<<std::endl;
		return -1;
	}
	
	return 0;
}

//Draws the specified texture
//param:texture->the index given from load texture
//param:color->the tint to apply to the texture
//param:sourceRect->the rectangle to pull a smaller image from the texture with. NULL for the whole texture
//param:position->the point to draw to
//param:angle->the rotation to apply
//param:scale->the scale to apply. This is uniform across x and y
//param:flip->SDL_RendererFlip::None/FlipHorizontal/FlipVertical. If you want both Flip Horizontal and Flip Vertical,
//use the bitwise operator '|'
//param:origin->Origin to rotate around. If NULL, uses center of destRect created by position and scale
//param:layerDepth->The depth to draw the image at
//returns -1 on error, 0 for success
int SpriteBatch::DrawTexture(Uint32 texture, SDL_Color color, const SDL_Rect* sourceRect, const SDL_Point* position,
		float angle, float scale, SDL_RendererFlip flip, const SDL_Point* origin, float layerDepth)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	///create a temporary point
	SDL_Point pos = SDL_Point();

	//if position is null, set our temp to 0,0
	if(!position)
	{
		pos = CreatePoint(0, 0);
	}
	else
	{
		//otherwise set with position
		pos = CreatePoint(position->x, position->y);
	}

	SDL_Rect destRect = SDL_Rect();

	//If we were given a source rectangle
	if(sourceRect)
	{
		//create a dest rectangle using position and source rectangle's dimensions
		destRect = CreateRect(pos.x, pos.y, (int)(sourceRect->w * scale), (int)(sourceRect->h * scale));
	}
	else
	{
		int w = 0;
		int h = 0;

		//get the width and height
		SDL_QueryTexture(textureList[texture], NULL, NULL, &w, &h);
		//create a dest rect using position and the image dimensions
		destRect = CreateRect(pos.x, pos.y, (int)(w * scale), (int)(h * scale));
	}

	//Call other DrawTexture to do the actual drawing
	return DrawTexture(texture, color, sourceRect, &destRect, angle, origin, flip, layerDepth);
}

//Draw the texture from the PackedSprite structure
//param:packedSprite->The packed sprite to draw from
void SpriteBatch::DrawTexture(PackedSprite packedSprite)
{
	if(packedSprite.Type == PackedSprite::PackType::Sprite)
	{
		//Get the texture (we dont do bounds checking because when we added the texture to the list
		//we checked the bounds) We also don't check for begin, because that would've been checked in the 
		//users call to DrawTexture. This is a private function. Only spritebatch calls this one.
		SDL_Texture* tex = textureList[packedSprite.Texture];

		//Apply any blends
		ApplyBlendToTexture(tex, packedSprite.Tint);

		//if no flip or angle, draw with Render Copy
		if(packedSprite.Rotation == 0 && packedSprite.FlipEffects == 0)
			SDL_RenderCopy(renderer, tex, packedSprite.SourceRect, packedSprite.DestRect);
		else
			///Draw the image with any flip/angle
			SDL_RenderCopyEx(renderer, tex, packedSprite.SourceRect, packedSprite.DestRect, 
				packedSprite.Rotation, packedSprite.Origin, packedSprite.FlipEffects);
	}
	else
	{
		SDL_Surface* textSurface;

		//Generate surface based on draw mode
		if(packedSprite.DrawMode == StringDrawMode::Blended)
		{
			if(packedSprite.WrapLength <= 0)					
				textSurface = TTF_RenderText_Blended(fontManager->GetFont(packedSprite.Texture), packedSprite.Message.c_str(), 
				packedSprite.Tint);
			else
				textSurface = TTF_RenderText_Blended_Wrapped(fontManager->GetFont(packedSprite.Texture), 
				packedSprite.Message.c_str(), packedSprite.Tint, packedSprite.WrapLength);
		}
		else if(packedSprite.DrawMode == StringDrawMode::Shaded)
		{
			textSurface = TTF_RenderText_Shaded(fontManager->GetFont(packedSprite.Texture), packedSprite.Message.c_str(), 
				packedSprite.Tint, packedSprite.BackgroundColor);
		}
		else if(packedSprite.DrawMode == StringDrawMode::Solid)
		{
			textSurface = TTF_RenderText_Solid(fontManager->GetFont(packedSprite.Texture), packedSprite.Message.c_str(), 
				packedSprite.Tint);
		}

		//Generate the texture from the surface
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, textSurface);

		//Free the surface and get rid of the pointer to it
		SDL_FreeSurface(textSurface);
		textSurface = nullptr;

		int w = 0;
		int h = 0;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);

		//Create a rectangle that is the size of the string
		SDL_Rect sourceRect = CreateRect(0, 0, w, h);
		SDL_Rect destRect = CreateRect(packedSprite.DestRect->x, packedSprite.DestRect->y,
			(Uint32)(w * packedSprite.StringScale), (Uint32)(h * packedSprite.StringScale));
		
		//if no flip or angle, draw with Render Copy
		if(packedSprite.Rotation == 0 && packedSprite.FlipEffects == SDL_RendererFlip::SDL_FLIP_NONE)
			SDL_RenderCopy(renderer, tex, &sourceRect, &destRect);
		else
			///Draw the image with any flip/angle
			SDL_RenderCopyEx(renderer, tex, &sourceRect, &destRect, 
				packedSprite.Rotation, packedSprite.Origin, packedSprite.FlipEffects);

	}
}

//Draws a string to the screen
//param:font->The index of the font
//param:message->The message to display
//param:drawMode->The draw mode to apply to the string:
//Blended: Blends the text with whatever is behind it using transparency. This mode is required for wrapping. Slowest mode
//Shaded: Gives the text a background that utilizes background color
//Solid: Fastest rendering speed. No box around, just quick straight text
//param:foregroundColor->The color of the text
//param:backgroundColor->The color of the background box. Used with Shaded drawmode
//param:wrapLength->Wraps the text past this width (in pixels). Requires empty space, will not separate words. Only
//works in Blended mode
//param:position->Point to draw the text at
//param:angle->Angle to rotate the text
//param:origin->The origin point to rotate about
//param:scale->Amount to size the text. Will distort quickly
//param:flip-> Flip texture horizontally, vertically, both or none
//param:layerDepth->The depth to draw on
//Returns 0 for success, -1 for errors
int SpriteBatch::DrawString(Uint32 font, std::string message, StringDrawMode drawMode, SDL_Color foregroundColor,
		SDL_Color backgroundColor, int wrapLength, const SDL_Point* position, float angle, const SDL_Point* origin, 
		float scale, SDL_RendererFlip flip, float layerDepth)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}
	//make sure we have a valid layer depth
	if(layerDepth < 0 || layerDepth > 1)
	{
		std::cout <<"Layer Depth must be between 0 and 1"<<std::endl;
		return -1;
	}

	//Make sure we have a valid renderer
	if(!renderer)
	{	
		std::cout<<"Renderer is null. Did you Initialize Spritebatch?"<<std::endl;
		return -1;	
	}
	
	try{
		TTF_Font* fontToDraw;
		//Get the texture (at performs bounds checking, which will validate the index for us, regardless of sort mode)		
		fontToDraw = fontManager->GetFont(font);

		//If sort mode is immediate, draw right away
		if(sortMode == SpriteBatchSortMode::Immediate)
		{
			SDL_Surface* textSurface = nullptr;
			//Create surface based on draw mode
			if(drawMode == StringDrawMode::Blended)
			{
				if(wrapLength <= 0)					
					textSurface = TTF_RenderText_Blended(fontToDraw, message.c_str(), foregroundColor);
				else
					textSurface = TTF_RenderText_Blended_Wrapped(fontToDraw, message.c_str(), foregroundColor, wrapLength);
			}
			else if(drawMode == StringDrawMode::Shaded)
			{
				textSurface = TTF_RenderText_Shaded(fontToDraw, message.c_str(), foregroundColor, backgroundColor);
			}
			else if(drawMode == StringDrawMode::Solid)
			{
				textSurface = TTF_RenderText_Solid(fontToDraw, message.c_str(), foregroundColor);
			}

			//Generate the texture from the surface
			SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, textSurface);

			//Free the surface and get rid of the pointer to it
			SDL_FreeSurface(textSurface);
			textSurface = nullptr;

			int w = 0;
			int h = 0;
			SDL_QueryTexture(tex, NULL, NULL, &w, &h);
			//Check for null position
			SDL_Point point;

			if(position == nullptr)
				point = CreatePoint(0, 0);
			else
				point = CreatePoint(position->x, position->y);

			//Create a rectangle that is the size of the string
			SDL_Rect sourceRect = CreateRect(0, 0, w, h);
			SDL_Rect destRect = CreateRect(point.x, point.y, (int)(w * scale), (int)(h * scale));
			
			//if no flip or angle, draw with Render Copy
			if(angle == 0 && flip == 0)
				SDL_RenderCopy(renderer, tex, &sourceRect, &destRect);
			else
				///Draw the image with any flip/angle
				SDL_RenderCopyEx(renderer, tex, &sourceRect, &destRect, angle, origin, flip);
		}
		else
		{
			//Take the layer depth, multiply by 10 and floor it. A layer depth of 0.09 would then go into
			//index 0 map, while a 0.99 will go into index 9
			int sortIndex = (int)std::floor(layerDepth * 10);
			SDL_Rect* destRect = &CreateRect(position->x, position->y, 0, 0);
			SDL_Rect sourceRect = CreateRect(0, 0, 0, 0);

			//Package the information
			PackedSprite package = PackedSprite(PackedSprite::PackType::String, font, message, drawMode,
				destRect, origin, backgroundColor, foregroundColor, flip, angle, scale, wrapLength);

			//if sort index is greater than 9, put it in array slot 9. We do this because 
			//1.0 * 10 = floor(10) = 10. Our array only goes to the 9th index
			if(sortIndex >= 9)
			{
				sortedImages[9].insert(std::pair<float, PackedSprite>(layerDepth, package));
			}
			else
			{
				//otherwise, just put it in the appropriate slot
				sortedImages[sortIndex].insert(std::pair<float, PackedSprite>(layerDepth, package));
			}
		}
	}
	//If invalid index, we throw the error
	catch (std::out_of_range problem)
	{
		std::cout<<"SPRITEBATCH_DRAWSTRING ERROR: Font not a valid index"<<std::endl;
		return -1;
	}
	
	return 0;
}

//Draws a line between two points
//param:x1->X coordinate of point 1
//param:y1->Y coordinate of point 1
//param:x2->X coordinate of point 2
//param:y2->Y coordinate of point 2
//param:color->The color to draw the line with
int SpriteBatch::DrawLine(int x1, int y1, int x2, int y2, SDL_Color color)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	int result = 0;

	//Set the line color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	//Check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawLine error: Problem setting Line Color"<<std::endl;
		return result;
	}

	//Draw the line
	result = SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

	//Check for line drawing problems
	if(result != 0)
	{
		std::cout<<"DrawLine error: Problem drawing line"<<std::endl;
		return result;
	}

	//Return success
	return result;
}

//Draws a line between two points
//param:point1->The point to start the line from
//param:point2->The point to end the line at
//param:color->The color to draw the line with
//Returns 0 on success, -1 for errors
int SpriteBatch::DrawLine(SDL_Point point1, SDL_Point point2, SDL_Color color)
{
	return DrawLine(point1.x, point1.y, point2.x, point2.y, color);
}

//Draws lines between the array of points
//param:pointsArray->The array of points to draw lines between
//param:numberOfPoints->The length of the array
//param:connectEndToStart->True to have the last point connect to the first point
int SpriteBatch::DrawLines(SDL_Point* pointsArray, int numberOfPoints,SDL_Color color, bool connectEndToStart)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	int result = 0;
	
	//Check for proper size
	if(numberOfPoints < 2)
	{
		std::cout<<"DrawLines Error: Number Of Points must be greater than 2"<<std::endl;
		return -1;
	}

	//Set line color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	
	//check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawLines error: Problem setting lines' color"<<std::endl;
		return result;
	}	

	//Draw all the lines
	result = SDL_RenderDrawLines(renderer, pointsArray, numberOfPoints);

	//Check for draw errors
	if(result != 0)
	{
		std::cout<<"DrawLines error" <<std::endl;
		return result;
	}

	//If true to connect end to start
	if(connectEndToStart)
	{
		//Draw a line from first point to last point
		result = SDL_RenderDrawLine(renderer, pointsArray[0].x, pointsArray[0].y, 
			pointsArray[numberOfPoints - 1].x, pointsArray[numberOfPoints-1].y);

		//Check for draw errors
		if(result != 0)
		{
			std::cout<<"DrawLines error: Can't connect last element to beginning element"<<std::endl;
			return result;
		}
	}

	//return success
	return result;
}

//Draws a point at the specified coordinates
//param:x->X coordinate of the point
//param:y->Y coordinate of the point
//param:color->Color to draw the point
//returns 0 for success, -1 for errors
int SpriteBatch::DrawPoint(int x, int y, SDL_Color color)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	int result = 0;

	//Set the point color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	//Check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawPoint error: Problem setting point color"<<std::endl;
		return result;
	}

	//Draw the point
	result = SDL_RenderDrawPoint(renderer, x, y);

	//Check for drawing problems
	if(result != 0)
	{
		std::cout<<"DrawPoint error: Problem drawing the point"<<std::endl;
		return result;
	}

	//return success
	return result;
}

//Draws a point at the specified point
//param:point->The point to draw at
//param:color->The color to draw the point
//returns 0 for success, -1 for errors
int SpriteBatch::DrawPoint(SDL_Point point, SDL_Color color)
{
	return DrawPoint(point.x, point.y, color);
}

//Draws points at the specified positions
//param:pointsArray->The array to draw from
//param:numberOfPoints->The size of the array. No bounds checking performed
//param:color->The color to set the points to
//Returns 0 for success, -1 for errors
int SpriteBatch::DrawPoints(SDL_Point* pointsArray, int numberOfPoints, SDL_Color color)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	//Check the size
	if(numberOfPoints<= 0)
	{
		std::cout<<"DrawPoints error: Number of points must be greater than 0" <<std::endl;
		return -1;
	}

	int result = 0;
	//set the color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	//check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawPoints error: Problem setting points color"<<std::endl;
		return result;
	}	

	//Draw the points
	result = SDL_RenderDrawPoints(renderer, pointsArray, numberOfPoints);

	//Check for draw problems
	if(result != 0)
	{
		std::cout<<"DrawPoints error: Problem drawing points"<<std::endl;
		return result;
	}

	//return success
	return result;
}

//Draws a rectangle to the screen
//param:rect->The rectangle to draw
//param:color->The color to draw the rectangle with
//returns 0 on success, -1 on error
int SpriteBatch::DrawRect(SDL_Rect* rect, SDL_Color color)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	int result = 0;

	//Set the rectangle color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	//Check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawRect error: Problem setting rectangle color" << std::endl;
		return result;
	}

	//Draw the rectangle
	result = SDL_RenderDrawRect(renderer, rect);

	//Check for rectangle drawing problems
	if(result != 0)
	{
		std::cout << "DrawRect error: Problem drawing rectangle" << std::endl;
		return result;
	}

	//return success
	return result;
}

//Draws a rectangle to the screen
//param:x->X coordinate of rectangle
//param:y->Y coordinate of rectangle
//param:w->Width of rectangle
//param:h->Height of rectangle
//param:color->Color of rectangle
//returns 0 on success, -1 on error
int SpriteBatch::DrawRect(int x, int y, Uint32 w, Uint32 h, SDL_Color color)
{
	SDL_Rect* rect = new SDL_Rect();
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;

	return DrawRect(rect, color);
}

//Draw a filled rectangle
//param:rectangle->The rectangle to fill
//param:color->The color to fill the rectangle with
//returns 0 for success, -1 for error
int SpriteBatch::DrawFilledRect(SDL_Rect* rectangle, SDL_Color color)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	int result = 0;

	//Set draw color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	
	//Check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawFilledRect error: Problem setting rectangle color" << std::endl;
		return result;
	}

	//Draw filled rectangle
	result = SDL_RenderFillRect(renderer, rectangle);

	//Check for draw errors
	if(result != 0)
	{
		std::cout<<"DrawFilledRect error: Problem drawing filled rectangle"<<std::endl;
		return result;
	}

	//return result
	return result;
}

//Draws an array of rectangles
//param:rectangles->The rectangles to draw
//param:numberOfRectangles->The number of rectangles in the array
//param:color->The color to draw the rectangles
//returns 0 for success, -1 for errors
int SpriteBatch::DrawRects(SDL_Rect* rectangles, int numberOfRectangles, SDL_Color color)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	//Make sure number of rectangles is valid
	if(numberOfRectangles <= 0)
	{
		std::cout<<"Draw Rects error: Size must be greater than 0"<<std::endl;
		return -1;
	}

	int result = 0;

	//Set the draw color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	//Check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawRects error: Problem setting rectangle color" << std::endl;
		return result;
	}

	//Draw the rectangles
	result = SDL_RenderDrawRects(renderer, rectangles, numberOfRectangles);

	//Check for draw problems
	if(result !=0)
	{
		std::cout<<"DrawRects error: Problem drawing rectangles"<<std::endl;
		return result;
	}

	//Return success
	return result;
}

//Draws multiple filled rectangles
//param:rectangles->pointer to an array of rectangles
//param:numberOfRectangles->Number of rectangles in the array
//param:color->The color to fill the rectangles with
//returns 0 for success, -1 for error
int SpriteBatch::DrawFilledRects(SDL_Rect* rectangles, int numberOfRectangles, SDL_Color color)
{
	//Check if spritebatch.begin has been called
	if(!begun)
	{
		std::cout<<"Begin must be called before attempting to draw"<<std::endl;
		return -1;
	}

	//Make sure number of rectangles is valid
	if(numberOfRectangles <= 0)
	{
		std::cout<<"DrawFilledRects error: Size must be greater than 0"<<std::endl;
		return -1;
	}

	int result = 0;

	//Set the draw color
	result = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	//Check for color setting problems
	if(result != 0)
	{
		std::cout<<"DrawFilledRects error: Problem setting rectangle color" << std::endl;
		return result;
	}

	result = SDL_RenderFillRects(renderer, rectangles, numberOfRectangles);

	if(result != 0)
	{
		std::cout<<"DrawFilledRects error: Problem drawing rectangles"<<std::endl;
		return result;
	}

	return result;
}

//Draws an outlined filled rectangle
//param:rectangle->The rectangle to draw
//param:outlineColor->The color of the outline
//param:fillColor->The color to fill it with
//returns 0 for success, -1 for errors
int SpriteBatch::DrawOutlinedFilledRect(SDL_Rect* rectangle, SDL_Color outlineColor, SDL_Color fillColor)
{
	int result = 0;

	//Draw the filled rectangle
	result = DrawFilledRect(rectangle, fillColor);

	//Check for filled rectangle drawing errors
	if(result != 0)
	{
		std::cout<<"DrawOutlinedFilledRect error: Error drawing filled rectangle"<<std::endl;
		return result;
	}

	//Draw outlined rectangle
	result = DrawRect(rectangle, outlineColor);

	//Check for outline rectangle drawing errors
	if(result != 0)
	{
		std::cout<<"DrawOutlinedFilledRect error: Error drawing outline rectangle"<<std::endl;
		return result;
	}

	//Return success
	return result;
}

//Draws an array of rectangles with an outline color and fill color
//param:rectangles->The array of rectangles
//param:numberOfRectangles->The size of the array
//param:outlineColor->The color to outline the rectangles with
//param:fillColor->The color to fill the rectangles with
//returns 0 for success, -1 for errors
int SpriteBatch::DrawOutlinedFilledRects(SDL_Rect* rectangles, int numberOfRectangles, SDL_Color outlineColor, SDL_Color fillColor)
{
	int result = 0;

	//Draw the filled rectangle
	result = DrawFilledRects(rectangles, numberOfRectangles, fillColor);

	//Check for filled rectangle drawing errors
	if(result != 0)
	{
		std::cout<<"DrawOutlinedFilledRect error: Error drawing filled rectangle"<<std::endl;
		return result;
	}

	//Draw outlined rectangle
	result = DrawRects(rectangles, numberOfRectangles, outlineColor);

	//Check for outline rectangle drawing errors
	if(result != 0)
	{
		std::cout<<"DrawOutlinedFilledRect error: Error drawing outline rectangle"<<std::endl;
		return result;
	}

	//Return success
	return result;
}

//Sets the Texture Blend Mode with the appropriate setting and color
int SpriteBatch::ApplyBlendToTexture(SDL_Texture* texture, SDL_Color color)
{
	int returnResult = 0;

	//if no blending, skip the function
	if(blendMode == SDL_BLENDMODE_NONE)
		return 0;

	//if texture is null, throw error and return
	if(!texture)
	{
		std::cout<<"SetTextureBlendMode error: Texture is null"<<std::endl;
		return -1;
	}

	//Set blend mode
	if(SDL_SetTextureBlendMode(texture, blendMode) != 0)
	{
		std::cout<<"Set Blend Mode error: Blend Mode may not be supported"<<std::endl;
		returnResult = -1;
	}

	//If there is tinting, apply it
	if(color != COLOR_WHITE)
	{
		//Apply tint
		if(SDL_SetTextureColorMod(texture, color.r, color.g, color.b)!=0)
		{
			std::cout<<"Set Tint error: Tinting may not be supported"<<std::endl;
			returnResult = -1;
		}
	
		//Apply alpha modification	
		if(SDL_SetTextureAlphaMod(texture, color.a) != 0)
		{
			std::cout<<"Set Alpha error: Alpha modification may not be supported" <<std::endl;
			returnResult = -1;
		}
	}
	return returnResult;
}

//Ends the Spritebatch
int SpriteBatch::End()
{
	//if Begin wasn't called, throw an error
	if(!begun)
	{
		std::cout<< "End has been called before Begin. Call Begin before calling End"<< std::endl;
		return -1;
	}
	else
	{
		//if we are drawing back to front
		if(sortMode == SpriteBatchSortMode::BackToFront)
		{
			//Loop through the array
			for(int i = 9; i >= 0; i--)
			{
				//if the current map has data
				if(sortedImages[i].size() > 0)
				{
					//Create a reverse iterator (to iterate from back to front)
					std::multimap<float, PackedSprite>::reverse_iterator pos;
					
					//Initialize the reverse iterator loop
					for(pos = sortedImages[i].rbegin(); pos != sortedImages[i].rend(); ++pos)
					{
						//Grab the value from the <key,value> pair and draw it						
						DrawTexture(pos->second);
					}
					
					//Clear the map
					sortedImages[i].clear();
				}
			}
		}
		//if the sort mode is from front to back
		else if(sortMode == SpriteBatchSortMode::FrontToBack)
		{
			//Loop through the array
			for(int i = 0; i <= 9; i++)
			{
				//if the current map has data
				if(sortedImages[i].size() > 0)
				{
					//Create an iterator
					std::multimap<float, PackedSprite>::iterator pos;
					
					//Initialize the iterator loop
					for(pos = sortedImages[i].begin(); pos != sortedImages[i].end(); ++pos)
					{
						//Grab the value from the <key,value> pair and draw it						
						DrawTexture(pos->second);
					}
					//Clear the map
					sortedImages[i].clear();
				}
			}
		}

		PresentRenderer();
		begun = false;

		return 0;
	}
}

//Presents the finished drawing
int SpriteBatch::PresentRenderer()
{
	if(renderer)
		SDL_RenderPresent(renderer);
	else
	{	
		std::cout<<"Renderer is null. Did you Initialize Spritebatch?"<<std::endl;
		return -1;	
	}

	return 0;
}


SDL_Texture* SpriteBatch::GetTexture(int texture)
{
 try
 {
  SDL_Texture* textureQueried = textureList.at(texture);
  
  return textureQueried;
 }
 //If invalid index, we throw the error
 catch (std::out_of_range problem)
 {
  std::cout<<"SPRITEBATCH_QUERY TEXTURE ERROR: Texture not a valid index"<<std::endl;
  return NULL;
 }
}


SDL_Rect SpriteBatch::QueryTexture(int texture)
{
 try
 {
  SDL_Texture* textureQueried = textureList.at(texture);

  int w, h;

  SDL_QueryTexture(textureQueried, NULL, NULL, &w, &h);

  return CreateRect(0, 0, w, h);
 }
 //If invalid index, we throw the error
 catch (std::out_of_range problem)
 {
  std::cout<<"SPRITEBATCH_QUERY TEXTURE ERROR: Texture not a valid index"<<std::endl;
  return CreateRect(0, 0, 0, 0);
 }
}