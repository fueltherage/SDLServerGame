#pragma once
#include <SDL.h>
#include <string>

enum StringDrawMode{Blended, Shaded, Solid};

struct PackedSprite
{
	enum PackType{Sprite, String};
	PackType Type;
	Uint32 Texture;
	std::string Message;
	const SDL_Rect* SourceRect;
	const SDL_Rect* DestRect;
	const SDL_Point* Origin;
	SDL_Color Tint;
	SDL_Color BackgroundColor;
	SDL_RendererFlip FlipEffects;
	float Rotation;
	float StringScale;
	int WrapLength;
	StringDrawMode DrawMode;

	PackedSprite(PackType type, Uint32 index, const SDL_Rect* sourceRect, const SDL_Rect* destRect, 
		const SDL_Point* origin, SDL_Color tint,  SDL_RendererFlip flip, float rotation)
	{
		Type = type;
		Texture = index;
		SourceRect = sourceRect;
		DestRect = destRect;
		Origin = origin;
		Tint = tint;
		FlipEffects = flip;
		Rotation = rotation;
	}

	PackedSprite(PackType type, Uint32 index, std::string message, StringDrawMode drawMode, 
		const SDL_Rect* destRect, const SDL_Point* origin, SDL_Color backgroundColor, 
		SDL_Color foregroundColor, SDL_RendererFlip flip, float rotation, float stringScale, int wrapLength)
	{
		Type = type;
		Texture = index;
		Message = message;
		DrawMode = drawMode;
		SourceRect = NULL;
		DestRect = destRect;
		Origin = origin;
		Tint = foregroundColor;
		BackgroundColor = backgroundColor;
		FlipEffects = flip;
		Rotation = rotation;
		StringScale = stringScale;
		WrapLength = wrapLength;
	}

	~PackedSprite()
	{
	}
};