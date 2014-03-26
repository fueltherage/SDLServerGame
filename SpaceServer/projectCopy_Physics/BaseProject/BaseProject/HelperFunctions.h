#pragma once
#include <SDL.h>

//Acts as a constructor for a point
//param:x->The x coordinate
//param:y->The y coordinate
inline SDL_Point CreatePoint(int x, int y);

//Acts as a constructor for a color. Parameters should be between
//0 and 255. No bounds checking is performed
//param:r->The red value
//param:g->The green value
//param:b->The blue value
//param:a->The alpha value
//Returns an SDL_Color
inline SDL_Color CreateColor(int r, int g, int b, int a = 255);

//Acts as a constructor for a color. Parameters should be between
//0 and 1. Use other overload to avoid unneccessary calculations.
//No bounds checking is performed
//param:r->The red value
//param:g->The green value
//param:b->The blue value
//param:a->The alpha value
//Returns an SDL_Color
inline SDL_Color CreateColor(float r, float g, float b, float a = 1.0f);

//Acts as a constructor for a Rectangle
//param:x->The x coordinate of the rectangle
//param:y->The y coordinate of the rectangle
//param:w->The width of the rectangle
//param:h->The height of the rectangle
//Returns a rectangle with the given specifications
inline SDL_Rect CreateRect(int x, int y, Uint32 w, Uint32 h);

//Acts as a constructor for a point
//param:x->The x coordinate
//param:y->The y coordinate
SDL_Point CreatePoint(int x, int y)
{
	SDL_Point point = SDL_Point();
	point.x = x;
	point.y = y;

	return point;
}

//Acts as a constructor for a color. Parameters should be between
//0 and 255. No bounds checking is performed
//param:r->The red value
//param:g->The green value
//param:b->The blue value
//param:a->The alpha value
//Returns an SDL_Color
SDL_Color CreateColor(int r, int g, int b, int a)
{
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;

	return color;
}

//Acts as a constructor for a color. Parameters should be between
//0 and 1. Use other overload to avoid unneccessary calculations.
//No bounds checking is performed
//param:r->The red value
//param:g->The green value
//param:b->The blue value
//param:a->The alpha value
//Returns an SDL_Color
SDL_Color CreateColor(float r, float g, float b, float a)
{
	//Multiply all the float values by 255 to get the proper numbers

	return CreateColor((int)(r * 255), (int)(g * 255), (int)(b * 255), (int)(a * 255));
}

//Acts as a constructor for a Rectangle
//param:x->The x coordinate of the rectangle
//param:y->The y coordinate of the rectangle
//param:w->The width of the rectangle
//param:h->The height of the rectangle
//Returns a rectangle with the given specifications
SDL_Rect CreateRect(int x, int y, Uint32 w, Uint32 h)
{
	SDL_Rect rect = SDL_Rect();
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	return rect;
}

