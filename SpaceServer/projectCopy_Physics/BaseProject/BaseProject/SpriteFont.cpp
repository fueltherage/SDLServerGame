#include "SpriteFont.h"
#include <iostream>

static SpriteFont* instance;

SpriteFont::SpriteFont()
{
	//Reserve 5 slots for the font list
	fontList.reserve(5);
}

SpriteFont::~SpriteFont(void)
{
	//if we have any fonts, delete them
	if(fontList.size() > 0)
	{
		while(fontList.size() > 0)
		{
			TTF_CloseFont(fontList.back());
			fontList.pop_back();
		}
	}
}

//Deletes this instance of spritefont and sets the pointer to null. Only Spritebatch should call this
//param:spriteFont->The pointer to this spritefont instance
void SpriteFont::DestroySpriteFont(SpriteFont* &spriteFont)
{
	delete instance;
	spriteFont = nullptr;
}

//Gets the spritefont instance
SpriteFont* SpriteFont::GetInstance()
{
	if(instance == nullptr)
	{
		instance = new SpriteFont();
	}

	return instance;
}

//Loads a Font from a .ttf or .fon file format
//param:fileName->Filepath to load from
//param:fontSize->Size of the font to use. If size doesn't match a size listed in the file
//it will default to the size that is closest
//Returns index to the font, otherwise -1 for errors
int SpriteFont::LoadFont(std::string fileName, int fontSize)
{
	//variable to store font
	TTF_Font* font = nullptr;

	//open the font
	font = TTF_OpenFont(fileName.c_str(), fontSize);

	//Check for errors
	if(!font)
	{
		std::cout<<"LoadFont error: Error loading font."<<std::endl;
		return -1;
	}

	//Add to the font list
	fontList.push_back(font);

	//return the index
	return fontList.size() -1;
}

//Sets the font style of the font at the specified index
//param:fontIndex->The index of the font to modify
//param:style->The style to set it to. Use | to use multiple
//TTF_STYLE_BOLD: Sets the font to its bold version
//TTF_STYLE_ITALIC: Italicizes the font
//TTF_STYLE_NORMAL: Sets the font to its normal format
//TTF_STYLE_STRIKETHROUGH: Creates a line through the middle
//TTF_STYLE_UNDERLINE: Creates an underline
void SpriteFont::SetFontStyle(int fontIndex, int style)
{
	//Try block to make sure index is valid
	try
	{
		//Get the font to modify
		TTF_Font* font = fontList.at(fontIndex);

		//Check for null
		if(font)
		{
			//Get the current style
			int currentStyle = TTF_GetFontStyle(font);

			//Save processing by not changing style if they are identical
			if(currentStyle != style)
			{
				//Otherwise, set the style
				TTF_SetFontStyle(font, style);
			}
		}
		else
		{
			std::cout<<"SetFontStyle error: Provided index is NULL"<<std::endl;
			return;
		}
	}
	catch(std::out_of_range problem)
	{
		//display invalid index
		std::cout<<"SetFontStyle error: font index invalid"<<std::endl;
		return;
	}
}

//Sets the outline around the font
//param:fontIndex->The index of the font to modify
//param:outlineSize->The size to set the outline
void SpriteFont::SetFontOutline(int fontIndex, int outlineSize)
{
	//Try block to make sure index is valid
	try
	{
		//Get the font to modify
		TTF_Font* font = fontList.at(fontIndex);

		if(font)
		{
			//Get the current style
			int currentOutline = TTF_GetFontOutline(font);

			//Save processing by not changing style if they are identical
			if(currentOutline != outlineSize)
			{
				//Otherwise, set the style
				TTF_SetFontOutline(font, outlineSize);
			}		
		}
		else
		{
			std::cout<<"SetFontOutline error: Provided index is NULL"<<std::endl;
			return;
		}
	}
	catch(std::out_of_range problem)
	{
		//display invalid index
		std::cout<<"SetFontOutline error: font index invalid"<<std::endl;
		return;
	}
}

//Set the hinting of the value
//param:fontIndex->The index of the font
//param:hintingStyle->The style of hinting to apply
//TTF_HINTING_NORMAL: Normal hinting
//TTF_HINTING_LIGHT: Light hinting
//TTF_HINTING_MONO: Monochromatic hinting
//TTF_HINTING_NONE: No hinting
void SpriteFont::SetFontHinting(int fontIndex, int hintingStyle)
{
	//Try block to make sure index is valid
	try
	{
		//Get the font to modify
		TTF_Font* font = fontList.at(fontIndex);

		//Make sure its not null
		if(font)
		{
			//Get the current style
			int currentHint = TTF_GetFontHinting(font);

			//Save processing by not changing style if they are identical
			if(currentHint != hintingStyle)
			{
				//Otherwise, set the style
				TTF_SetFontHinting(font, hintingStyle);
			}
		}
		else
		{
			std::cout<<"SetFontHinting error: Provided index is NULL"<<std::endl;
			return;
		}
	}
	catch(std::out_of_range problem)
	{
		//display invalid index
		std::cout<<"SetFontOutline error: font index invalid"<<std::endl;
		return;
	}
}

//Sets the Kerning value of the font (basically, disable this if there are character overlaps)
//param:fontIndex->The index of the font to modify
//param:enableKerning->True to enable, false to disable
void SpriteFont::SetFontKerning(int fontIndex, bool enableKerning)
{
	//Try block to make sure index is valid
	try
	{
		//Get the font to modify
		TTF_Font* font = fontList.at(fontIndex);

		//Make sure its not null
		if(font)
		{			
			//Convert the bool to an integer. False = 0 (disable), non zero = true
			TTF_SetFontHinting(font, (int)enableKerning);			
		}
		else
		{
			std::cout<<"SetFontKerning error: Provided index is NULL"<<std::endl;
			return;
		}
	}
	catch(std::out_of_range problem)
	{
		//display invalid index
		std::cout<<"SetFontKerning error: font index invalid"<<std::endl;
		return;
	}
}

//Gets the size of the message when used with the specified font
//param:fontIndex->The index of the font to use
//param:message->The message to measure
//returns a pointer to an SDL_Point object. REMEMBER TO DELETE WHEN FINISHED
SDL_Point SpriteFont::GetMessageSize(int fontIndex, std::string message)
{
	//Check message size
	if(message.size() <= 0)
	{
		std::cout<<"GetMessageSize error: No message to measure"<<std::endl;
		return CreatePoint(0, 0);
	}

	//Try block to make sure index is valid
	try
	{
		//Get the font to measure
		TTF_Font* font = fontList.at(fontIndex);

		//Make sure its not null
		if(font)
		{			
			SDL_Point returnPoint = SDL_Point();
			//Size up the message
			TTF_SizeText(font, message.c_str(), &returnPoint.x, &returnPoint.y);

			return returnPoint;
		}
		else
		{
			std::cout<<"GetMessageSize error: Provided index is NULL"<<std::endl;
			return CreatePoint(0, 0);
		}
	}
	catch(std::out_of_range problem)
	{
		//display invalid index
		std::cout<<"GetMessageSize error: font index invalid"<<std::endl;
		return CreatePoint(0, 0);
	}
}

//Gets a TTF Font from the list of loaded fonts
//param:index->The index of the font to retrieve
//returns the font if successful. Null if not
TTF_Font* SpriteFont::GetFont(int index)
{
	try
	{
		return fontList.at(index);
	}
	catch(std::out_of_range problem)
	{
		std::cout<<"GetFont error: Index was out of range"<<std::endl;
		return NULL;
	}
}
