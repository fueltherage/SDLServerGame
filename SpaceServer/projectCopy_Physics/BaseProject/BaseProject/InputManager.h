#pragma once
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "GamepadInput.h"

class InputManager
{
public:
	enum InputTypes{
		Keyboard = 0x01,
		Mouse = 0x02,
		Gamepad = 0x04
	};
	
	~InputManager(void);

	//Destroys this instance of the input manager and sets the pointer to null
	//param:inputManagerPointer->The pointer to set to null
	void DestroyInputManager(InputManager* &inputManagerPointer);

	//Sets the type of input to update
	//param:typeToUpdate->The type of object to update
	void SetTypeToUpdate(unsigned char typeToUpdate);

	//Gets a pointer to the instance of the InputManager
	static InputManager* GetInstance();

	void Update();

	///KEYBOARD///

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


	///MOUSE///

	//Checks if a mouse button is currently in a down state
	//param:mouseButtonToCheck->The mouse button to evaluate. Use
	//SDL_BUTTON_*mouseButton*
	bool GetMouseButtonDown(Uint8 mouseButtonToCheck);
	
	//Checks if a mouse button is currently in an up state
	//param:mouseButtonToCheck->The mouse button to evaluate. Use
	//SDL_BUTTON_*mouseButton*
	bool GetMouseButtonUp(Uint8 mouseButtonToCheck);

	//Checks if a mouse button was pressed
	//param:mouseButtonToCheck->The mouse button to evaluate. Use
	//SDL_BUTTON_*mouseButton*
	bool GetMouseButtonPress(Uint8 mouseButtonToCheck);

	//Checks if the mouse was moved or not
	bool GetMouseMoved();

	//Gets the current mouse position and returns it as an SDL_Point
	//param:x->Pointer to value to store the x coordinate
	//param:y->Pointer to value to store the y coordinate
	//Returns an SDL_Point, even if x and y aren't null
	SDL_Point GetMousePosition(int* x = nullptr, int* y = nullptr);
	
	//Gets the relative mouse position (only filled when relative mode is enabled)
	//param:x->The pointer to the value to place the x-coordinate into
	//param:y->The pointer to the value to place the y-coordinate into
	//Returns the relative mouse position as an SDL_Point, even if x and y aren't null
	SDL_Point GetMouseRelativeModePosition(int* x = nullptr, int* y = nullptr);

	//Gets the mouse direction(not normalized)
	//param:x->Pointer to a float to set the x coordinate to
	//param:y->Pointer to a float to set the y coordinate to
	//returns an SDL_Point even if x and y aren't null
	SDL_Point GetMouseDirection(int* x = nullptr, int *y = nullptr);

	//Gets the amount that the wheel was scrolled in the two directions
	//param:x->Pointer to an int to store the x coordinate
	//param:y->Pointer to an int to store the y coordinate
	//Returns an SDL_Point with the coordinates, even if x and y are null
	SDL_Point GetMouseScroll(int* x = nullptr, int* y = nullptr);

	//Sets RelativeMouseMode. Check SDL_Documentation for effects
	//param:enabled->True to enable, false to disable
	void SetRelativeMouseMode(bool enable);

	//Sets the mouse position to the given coordinates of the window with current focus
	//param:x->X coordinate of the window to set it to
	//param:y->Y coordinate of the window to set it to
	void SetMousePosition(int x, int y);
	
	//Sets the mouse position to the given coordinates of the window with current focus
	//param:point->The point containing the coordinates of the window to set the mouse to
	void SetMousePosition(SDL_Point point);	

private:
	InputManager(void);

	KeyboardInput* keyboardInstance;
	MouseInput* mouseInstance;
	GamepadInput* gamepadInstance;

	unsigned char inputToManage;

	//Deletes all allocated memory by any input pointers, as well as set them to nullptr
	void DeleteAllInputPointers();
};

