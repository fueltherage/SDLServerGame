#include "KeyboardInput.h"

static KeyboardInput* instance;

bool currentKeyStates[323] = {false};
bool previousKeyStates[323] = {false};

KeyboardInput::KeyboardInput(void)
{
}


KeyboardInput::~KeyboardInput(void)
{
}

//Destroys the instance of keyboard input and sets the pointer to null
//param:pointerToKeyboard->The pointer to the instance of KeyboardInput
void KeyboardInput::DestroyKeyboardInput(KeyboardInput* &pointerToKeyboard)
{
	delete instance;
	pointerToKeyboard = nullptr;
}

//Gets the instance of the KeyboardInput
KeyboardInput* KeyboardInput::GetInstance()
{
	if(instance == nullptr)
		instance = new KeyboardInput();

	return instance;
}

void KeyboardInput::SetPreviousToCurrent()
{
	//Set the previous state to the current state
	for(int i = 0; i < 323; i++)
	{
		previousKeyStates[i] = currentKeyStates[i];
	}
}

//Updates the keyboard input with current key states
//param:keyboardEvent->The address to a keyboard event
void KeyboardInput::Update(SDL_Event &keyboardEvent)
{
	//Get the key to check
	SDL_Keycode key = keyboardEvent.key.keysym.scancode;

	//Set the keys to their appropriate states
	if(keyboardEvent.type == SDL_KEYDOWN)
	{
		currentKeyStates[key] = true;
	}
	if(keyboardEvent.type == SDL_KEYUP)
	{
		currentKeyStates[key] = false;
	}
	
}

//Check if the specified key is in a down state
//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
//returns true if it is down, false if not
bool KeyboardInput::GetKeyDown(Uint32 keyToCheck)
{
	return currentKeyStates[keyToCheck];
}

//Check if the specified key is in an up state
//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
//returns true if it is up, false if not
bool KeyboardInput::GetKeyUp(Uint32 keyToCheck)
{
	return !currentKeyStates[keyToCheck];
}

//Check if the specified key was pressed
//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
//returns true if it was pressed, false if not
bool KeyboardInput::GetKeyPress(Uint32 keyToCheck)
{
	return (previousKeyStates[keyToCheck] == true && currentKeyStates[keyToCheck] == false);
}
