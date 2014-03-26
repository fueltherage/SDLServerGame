#pragma once
#include<SDL.h>

class MouseInput
{
public:
	
	~MouseInput(void);

	//Gets the instance of this mouse input
	static MouseInput* GetInstance();
	
	//Destroys the mouse input instance and the pointer to it
	//param->mouseInputPointer->The pointer to the mouse input
	void DestroyMouseInput(MouseInput* &mouseInputPointer);

	//Sets the previous states to the last frames' current states
	void SetPreviousStates();

	//Update the Mouse Input
	//param:mouseEvent->The mouse event to poll
	void Update(SDL_Event &mouseEvent);

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
	MouseInput(void);
};

