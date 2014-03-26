#pragma once
#include<SDL.h>

class KeyboardInput
{
public:
	~KeyboardInput(void);

	//Destroys the instance of keyboard input and sets the pointer to null
	//param:pointerToKeyboard->The pointer to the instance of KeyboardInput
	void DestroyKeyboardInput(KeyboardInput* &pointerToKeyboard);

	//Gets the instance of the KeyboardInput
	static KeyboardInput* GetInstance();

	void Update(SDL_Event &keyboardEvent);

	void SetPreviousToCurrent();

	//Check if the specified key is in a down state
	//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
	//returns true if it is down, false if not
	bool GetKeyDown(Uint32 keyToCheck);

	//Check if the specified key is in an up state
	//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
	//returns true if it is up, false if not
	bool GetKeyUp(Uint32 keyToCheck);

	//Check if the specified key was pressed
	//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
	//returns true if it was pressed, false if not
	bool GetKeyPress(Uint32 keyToCheck);

private:
	KeyboardInput(void);


};

