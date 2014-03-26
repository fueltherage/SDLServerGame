#pragma once
#include <SDL_ttf.h>
#include <vector>
#include "HelperFunctions.h"
class SpriteFont
{
public:
	~SpriteFont(void);

	static SpriteFont* GetInstance();
	
	//Deletes this instance of spritefont and sets the pointer to null. Only Spritebatch should call this
	//param:spriteFont->The pointer to this spritefont instance
	void DestroySpriteFont(SpriteFont* &spriteFont);

	//Loads a Font from a .ttf or .fon file format
	//param:fileName->Filepath to load from
	//param:fontSize->Size of the font to use. If size doesn't match a size listed in the file
	//it will default to the size that is closest
	//Returns index to the font, otherwise -1 for errors
	int LoadFont(std::string fileName, int fontSize = 16);

	//Sets the font style of the font at the specified index
	//param:fontIndex->The index of the font to modify
	//param:style->The style to set it to. Use | to use multiple
	//TTF_STYLE_BOLD: Sets the font to its bold version
	//TTF_STYLE_ITALIC: Italicizes the font
	//TTF_STYLE_NORMAL: Sets the font to its normal format
	//TTF_STYLE_STRIKETHROUGH: Creates a line through the middle
	//TTF_STYLE_UNDERLINE: Creates an underline
	void SetFontStyle(int fontIndex, int style);
	
	//Sets the outline around the font
	//param:fontIndex->The index of the font to modify
	//param:outlineSize->The size to set the outline
	void SetFontOutline(int fontIndex, int outlineSize);

	//Set the hinting of the value
	//param:fontIndex->The index of the font
	//param:hintingStyle->The style of hinting to apply
	//TTF_HINTING_NORMAL: Normal hinting
	//TTF_HINTING_LIGHT: Light hinting
	//TTF_HINTING_MONO: Monochromatic hinting
	//TTF_HINTING_NONE: No hinting
	void SetFontHinting(int fontIndex, int hintingStyle);

	//Sets the Kerning value of the font (basically, disable this if there are character overlaps)
	//param:fontIndex->The index of the font to modify
	//param:enableKerning->True to enable, false to disable
	void SetFontKerning(int fontIndex, bool enableKerning);

	//Gets the size of the message when used with the specified font
	//param:fontIndex->The index of the font to use
	//param:message->The message to measure
	//returns a pointer to an SDL_Point object. REMEMBER TO DELETE WHEN FINISHED
	SDL_Point GetMessageSize(int fontIndex, std::string message);

	//Gets a TTF Font from the list of loaded fonts
	//param:index->The index of the font to retrieve
	//returns the font if successful. Null if not
	TTF_Font* GetFont(int index);

private:	
	SpriteFont() ;
	
	std::vector<TTF_Font*>fontList;//List to store fonts in
};