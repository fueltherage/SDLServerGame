#pragma once
#include "InputManager.h"
#include <iostream>

static InputManager* instance;

InputManager::InputManager(void)
{
	//Set all pointers to a null state
	keyboardInstance = nullptr;
	mouseInstance = nullptr;
	gamepadInstance = nullptr;

	SetTypeToUpdate(InputTypes::Keyboard | InputTypes::Mouse);	
}

//Gets a pointer to the instance of the InputManager
InputManager* InputManager::GetInstance()
{
	if(instance == nullptr)
		instance = new InputManager();

	return instance;
}

//De-allocates all memory
InputManager::~InputManager(void)
{
	DeleteAllInputPointers();
}

//Destroys this instance of the input manager and sets the pointer to null
//param:inputManagerPointer->The pointer to set to null
void InputManager::DestroyInputManager(InputManager* &inputManagerPointer)
{
	//if instance, delete it and set both to null
	if(instance)
	{
		delete instance;
		instance = nullptr;
		inputManagerPointer = nullptr;
	}
}

//Deletes all allocated memory by any input pointers, as well as set them to nullptr
void InputManager::DeleteAllInputPointers()
{
	//if the keyboard instance is used, destroy it
	if(keyboardInstance)
	{
		keyboardInstance->DestroyKeyboardInput(keyboardInstance);
	}

	//if the mouse instance is used, destroy it
	if(mouseInstance)
	{
		mouseInstance->DestroyMouseInput(mouseInstance);
	}	

	//if the gamepad instance is used, destroy it
	if(gamepadInstance)
	{
		gamepadInstance->DestroyGamepadInstance(gamepadInstance);
	}
}

//Sets the type of input to update
//param:typeToUpdate->The type of object to update
void InputManager::SetTypeToUpdate(unsigned char typeToUpdate)
{
	//Reset all inputs
	DeleteAllInputPointers();

	inputToManage = typeToUpdate;

	if(inputToManage & InputTypes::Keyboard)
		keyboardInstance = KeyboardInput::GetInstance();

	if(inputToManage & InputTypes::Mouse)
		mouseInstance = MouseInput::GetInstance();

	if(inputToManage & InputTypes::Gamepad)
		gamepadInstance = GamepadInput::GetInstance();
}

//Updates the inputs based on type of Input to poll
void InputManager::Update()
{
	//if the keyboard instance exists, update
	if(keyboardInstance)
		keyboardInstance->SetPreviousToCurrent();

	//if the mouse instance exists, update
	if(mouseInstance)
		mouseInstance->SetPreviousStates();

	SDL_Event inputEvent;
	//Check if we have a keyboard event to poll
	if(SDL_PollEvent(&inputEvent))
	{
		//If the keyboard is a type to manage
		if(inputToManage & InputTypes::Keyboard)
		{
			keyboardInstance->Update(inputEvent);
		}

		//If the mouse is a type to manage
		if(inputToManage & InputTypes::Mouse)
		{
			mouseInstance->Update(inputEvent);
		}
	}	
}

///KEYBOARD///

//Check if the specified key is in a down state
//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
//returns true if it is down, false if not
bool InputManager::GetKeyDown(Uint32 keyToCheck)
{
	//Make sure the keyboard is being polled
	if(inputToManage & InputTypes::Keyboard)
		return keyboardInstance->GetKeyDown(keyToCheck);
	else
	{
		std::cout<<"GetKeyUp error: The keyboard is not set as an input type to poll"<<std::endl;
		return false;
	}
}

//Check if the specified key is in an up state
//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
//returns true if it is up, false if not
bool InputManager::GetKeyUp(Uint32 keyToCheck)
{
	//Make sure the keyboard is being polled
	if(inputToManage & InputTypes::Keyboard)
		return keyboardInstance->GetKeyUp(keyToCheck);
	else
	{
		std::cout<<"GetKeyUp error: The keyboard is not set as an input type to poll"<<std::endl;
		return false;
	}
}

//Check if the specified key was pressed
//param:keyToCheck->The key to check. Use SDL_SCANCODE_*key*.
//returns true if it was pressed, false if not
bool InputManager::GetKeyPress(Uint32 keyToCheck)
{
	//Make sure the keyboard is being polled
	if(inputToManage & InputTypes::Keyboard)
		return keyboardInstance->GetKeyPress(keyToCheck);
	else
	{
		std::cout<<"GetKeyPress error: The keyboard is not set as an input type to poll"<<std::endl;
		return false;
	}
}

///KEYBOARD END///

///MOUSE///

//Checks if a mouse button is currently in a down state
//param:mouseButtonToCheck->The mouse button to evaluate. Use
//SDL_BUTTON_*mouseButton*
bool InputManager::GetMouseButtonDown(Uint8 mouseButtonToCheck)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMouseButtonDown(mouseButtonToCheck);
	else
	{
		std::cout<<"GetMouseButtonDown error: The mouse is not set as an input type to poll"<<std::endl;
		return false;	
	}
}
	
//Checks if a mouse button is currently in an up state
//param:mouseButtonToCheck->The mouse button to evaluate. Use
//SDL_BUTTON_*mouseButton*
bool InputManager::GetMouseButtonUp(Uint8 mouseButtonToCheck)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMouseButtonUp(mouseButtonToCheck);
	else
	{
		std::cout<<"GetMouseButtonUp error: The mouse is not set as an input type to poll"<<std::endl;
		return false;	
	}
}

//Checks if a mouse button was pressed
//param:mouseButtonToCheck->The mouse button to evaluate. Use
//SDL_BUTTON_*mouseButton*
bool InputManager::GetMouseButtonPress(Uint8 mouseButtonToCheck)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMouseButtonPress(mouseButtonToCheck);
	else
	{
		std::cout<<"GetMouseButtonPress error: The mouse is not set as an input type to poll"<<std::endl;
		return false;	
	}
}

//Checks if the mouse was moved or not
bool InputManager::GetMouseMoved()
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMouseMoved();
	else
	{
		std::cout<<"GetMouseMoved error: The mouse is not set as an input type to poll"<<std::endl;
		return false;	
	}
}

//Gets the current mouse position and returns it as an SDL_Point
//param:x->Pointer to value to store the x coordinate
//param:y->Pointer to value to store the y coordinate
//Returns an SDL_Point, even if x and y aren't null
SDL_Point InputManager::GetMousePosition(int* x, int* y)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMousePosition(x, y);
	else
	{
		std::cout<<"GetMousePosition error: The mouse is not set as an input type to poll"<<std::endl;
		SDL_Point point;
		point.x = 99999999;
		point.y = 99999999;
		return point;	
	}
}
	
//Gets the relative mouse position (only filled when relative mode is enabled)
//param:x->The pointer to the value to place the x-coordinate into
//param:y->The pointer to the value to place the y-coordinate into
//Returns the relative mouse position as an SDL_Point, even if x and y aren't null
SDL_Point InputManager::GetMouseRelativeModePosition(int* x, int* y)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMouseRelativeModePosition(x, y);
	else
	{
		std::cout<<"GetMouseRelativeModePosition error: The mouse is not set as an input type to poll"<<std::endl;
		SDL_Point point;
		point.x = 99999999;
		point.y = 99999999;
		return point;	
	}
}

//Gets the mouse direction(not normalized)
//param:x->Pointer to a float to set the x coordinate to
//param:y->Pointer to a float to set the y coordinate to
//returns an SDL_Point even if x and y aren't null
SDL_Point InputManager::GetMouseDirection(int* x, int *y)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMouseDirection(x, y);
	else
	{
		std::cout<<"GetMouseDirection error: The mouse is not set as an input type to poll"<<std::endl;
		SDL_Point point;
		point.x = 99999999;
		point.y = 99999999;
		return point;	
	}
}

//Gets the amount that the wheel was scrolled in the two directions
//param:x->Pointer to an int to store the x coordinate
//param:y->Pointer to an int to store the y coordinate
//Returns an SDL_Point with the coordinates, even if x and y are null
SDL_Point InputManager::GetMouseScroll(int* x, int* y )
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->GetMouseScroll(x, y);
	else
	{
		std::cout<<"GetMouseScroll error: The mouse is not set as an input type to poll"<<std::endl;
		SDL_Point point;
		point.x = 99999999;
		point.y = 99999999;
		return point;	
	}
}

//Sets RelativeMouseMode. Check SDL_Documentation for effects
//param:enabled->True to enable, false to disable
void InputManager::SetRelativeMouseMode(bool enable)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->SetRelativeMouseMode(enable);
	else
	{
		std::cout<<"SetRelativeMouseMode error: The mouse is not set as an input type to poll"<<std::endl;
		return;	
	}
}

//Sets the mouse position to the given coordinates of the window with current focus
//param:x->X coordinate of the window to set it to
//param:y->Y coordinate of the window to set it to
void InputManager::SetMousePosition(int x, int y)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->SetMousePosition(x, y);
	else
	{
		std::cout<<"SetMousePosition error: The mouse is not set as an input type to poll"<<std::endl;
		return;	
	}
}
	
//Sets the mouse position to the given coordinates of the window with current focus
//param:point->The point containing the coordinates of the window to set the mouse to
void InputManager::SetMousePosition(SDL_Point point)
{
	if(inputToManage & InputTypes::Mouse)
		return mouseInstance->SetMousePosition(point);
	else
	{
		std::cout<<"SetMousePosition error: The mouse is not set as an input type to poll"<<std::endl;
		return;	
	}
}

///MOUSE END///