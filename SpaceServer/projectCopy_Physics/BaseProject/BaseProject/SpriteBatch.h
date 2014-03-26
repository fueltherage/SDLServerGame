#pragma once
#include "PackedSprite.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SpriteFont.h"
#include <vector>
#include <map>

#ifndef SPRITEBATCH_CURRENT_VERSION
#define SPRITEBATCH_CURRENT_VERSION 1.0.0
#endif

//The possible sort modes to use
enum SpriteBatchSortMode{Immediate, FrontToBack, BackToFront};

class SpriteBatch
{
public:
	//Gets the instance of the current sprite batch
	static SpriteBatch* GetInstance();

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
	int Initialize(std::string title, int x, int y, int w, int h, Uint32 sdlFlag);
	//Initializes a window with the specified title and in the particular position & size
	int Initialize(std::string title, int x, int y, int w, int h);	
	//Initializes a window with the specified title and at the specified size
	int Initialize(std::string title, int w, int h);
	//Creates a window with the specified title. 
	//If fullscreen is true, sets window mode to fullscreen, else
	//Creates a window that is 100 pixels less than the desktop screen
	int Initialize(std::string title, bool fullScreen);
	
	//Sets the blend mode to use on points, lines, and rectangles
	//param:blendMode->The blendmode to apply
	int SetPrimitiveBlendMode(SDL_BlendMode blendMode);

	//Returns a texture from the passed file name. Requires Extension in the string
	//Returns -1 if errors, otherwise will return 0 or greater for success. This
	//successful int will be used to draw the textures later
	int LoadTexture(std::string fileName);
	//Loads an SDL Surface. Use Texture unless you know what you are doing
	//param:fileName->Filepath to load the surface from
	int LoadSurface(std::string fileName);	

	//Clear the renderer. Returns 0 if successful, -1 if not
	int ClearRenderer();

	//Prepares the Spritebatch for drawing
	//param spriteSorting->The sort mode to use for drawing
	//param blendMode->The way to blend the drawn textures
	//returns 0 if successful, -1 if not
	int Begin(SpriteBatchSortMode spriteSorting = SpriteBatchSortMode::Immediate, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE);

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
	int DrawTexture(Uint32 texture, SDL_Color color, const SDL_Rect* sourceRect = NULL, 
		 const SDL_Rect* destRect = NULL, float angle = 0.0, const SDL_Point* origin = NULL,
		SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE, float layerDepth = 1.0f );

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
	int DrawTexture(Uint32 texture, SDL_Color color, const SDL_Rect* sourceRect = NULL, const SDL_Point* position = NULL,
		float angle = 0.0f, float scale = 1.0f, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE, 
		const SDL_Point* origin = NULL,	float layerDepth = 1.0f);

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
	int DrawString(Uint32 font, std::string message, StringDrawMode drawMode, SDL_Color foregroundColor, SDL_Color backgroundColor, 
		int wrapLength, const SDL_Point* position = NULL, float angle = 0, const SDL_Point* origin = NULL, 
		float scale = 1.0f, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE, float layerDepth = 1.0f);

	//Draws a line between two points
	//param:x1->X coordinate of point 1
	//param:y1->Y coordinate of point 1
	//param:x2->X coordinate of point 2
	//param:y2->Y coordinate of point 2
	//param:color->The color to draw the line with
	int DrawLine(int x1, int y1, int x2, int y2, SDL_Color color);

	//Draws a line between two points
	//param:point1->The point to start the line from
	//param:point2->The point to end the line at
	//param:color->The color to draw the line with
	//Returns 0 on success, -1 for errors
	int DrawLine(SDL_Point point1, SDL_Point point2, SDL_Color color);

	//Draws lines between the array of points
	//param:pointsArray->The array of points to draw lines between
	//param:numberOfPoints->The length of the array. No bounds checking performed
	//param:connectEndToStart->True to have the last point connect to the first point
	//Returns 0 for success, -1 for errors
	int DrawLines(SDL_Point* pointsArray, int numberOfPoints, SDL_Color color, bool connectEndToStart = false);

	//Draws a point at the specified coordinates
	//param:x->X coordinate of the point
	//param:y->Y coordinate of the point
	//param:color->Color to draw the point
	//returns 0 for success, -1 for errors
	int DrawPoint(int x, int y, SDL_Color color);
	
	//Draws a point at the specified point
	//param:point->The point to draw at
	//param:color->The color to draw the point
	//returns 0 for success, -1 for errors
	int DrawPoint(SDL_Point pointToDraw, SDL_Color color);	

	//Draws points at the specified positions
	//param:pointsArray->The array to draw from
	//param:numberOfPoints->The size of the array. No bounds checking performed
	//param:color->The color to set the points to
	//Returns 0 for success, -1 for errors
	int DrawPoints(SDL_Point* pointsArray, int numberOfPoints, SDL_Color color);

	//Draws a rectangle to the screen
	//param:rect->The rectangle to draw
	//param:color->The color to draw the rectangle with
	//returns 0 on success, -1 on error
	int DrawRect(SDL_Rect* rect, SDL_Color color);

	//Draws a rectangle to the screen
	//param:x->X coordinate of rectangle
	//param:y->Y coordinate of rectangle
	//param:w->Width of rectangle
	//param:h->Height of rectangle
	//param:color->Color of rectangle
	//returns 0 on success, -1 on error
	int DrawRect(int x, int y, Uint32 w, Uint32 h, SDL_Color color);
	
	//Draw a filled rectangle
	//param:rectangle->The rectangle to fill
	//param:color->The color to fill the rectangle with
	//returns 0 for success, -1 for error
	int DrawFilledRect(SDL_Rect* rectangle, SDL_Color color);

	//Draws an array of rectangles
	//param:rectangles->The rectangles to draw
	//param:numberOfRectangles->The number of rectangles in the array
	//param:color->The color to draw the rectangles
	//returns 0 for success, -1 for errors
	int DrawRects(SDL_Rect* rectangles, int numberOfRectangles, SDL_Color color);

	//Draws multiple filled rectangles
	//param:rectangles->pointer to an array of rectangles
	//param:numberOfRectangles->Number of rectangles in the array
	//param:color->The color to fill the rectangles with
	//returns 0 for success, -1 for error
	int DrawFilledRects(SDL_Rect* rectangles, int numberOfRectangles, SDL_Color color);

	//Draws an outlined filled rectangle
	//param:rectangle->The rectangle to draw
	//param:outlineColor->The color of the outline
	//param:fillColor->The color to fill it with
	//returns 0 for success, -1 for errors
	int DrawOutlinedFilledRect(SDL_Rect* rectangle, SDL_Color outlineColor, SDL_Color fillColor);

	//Draws an array of rectangles with an outline color and fill color
	//param:rectangles->The array of rectangles
	//param:numberOfRectangles->The size of the array
	//param:outlineColor->The color to outline the rectangles with
	//param:fillColor->The color to fill the rectangles with
	//returns 0 for success, -1 for errors
	int DrawOutlinedFilledRects(SDL_Rect* rectangles, int numberOfRectangles, SDL_Color outlineColor, SDL_Color fillColor);

	//Ends the spritebatch
	int End();

	//Destroy's the instance of spritebatch as setting the pointer to it to null.
	//param:spriteBatch->Your pointer to spritebatch. 
	void DestroySpriteBatch(SpriteBatch* &spriteBatch);

	SDL_Rect QueryTexture(int texture);
	SDL_Texture* GetTexture(int texture);
	//Deletes all allocated memory
	~SpriteBatch(void);

private:
	SDL_Window* window;//The window to display from
	SDL_Renderer* renderer;//The renderer used to perform drawing

	bool begun;//True if spritebatch::begin has been called
	
	std::vector<SDL_Texture*>textureList;//List to store textures in
	std::vector<SDL_Surface*>surfaceList;//List to store surfaces in

	std::multimap<float, PackedSprite>* sortedImages;//Array of maps used for sorting if sortmode != immediate
	
	SpriteFont* fontManager;//Spritebatch's pointer to the Spritefont list

	SpriteBatchSortMode sortMode;//Sort mode used for drawing
	SDL_BlendMode blendMode;//Blend mode used for blending between textures

	//Constructor. Initializes SDL and SDL image
	SpriteBatch(void);
	
	//Presents the finished drawing
	int PresentRenderer();
	//Draw the texture from the PackedSprite structure
	//param:packedSprite->The packed sprite to draw from
	void DrawTexture(PackedSprite packedSprite);
	//Sets the Texture Blend Mode with the appropriate setting and color
	//param:texture->Texture to modify
	//param:color->Color to apply
	//returns 0 for success, -1 for errors
	inline int ApplyBlendToTexture(SDL_Texture* texture, SDL_Color color);
	
};

