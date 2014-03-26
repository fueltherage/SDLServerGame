#include "MouseInput.h"

static MouseInput* instance;

bool previousMButtonStates[5] = {false};//previous mouse button states
bool currentMButtonStates[5] = {false};//current mouse button states
SDL_Point currentMousePos;//current mouse position
SDL_Point currentMouseScroll;//current mouse scroll
SDL_Point currentRelativeMousePos;//Position for when RelativeMode is enabled
bool mouseMoved = false;//Check if the mouse was moved

MouseInput::MouseInput(void)
{
}


MouseInput::~MouseInput(void)
{
}

//Destroys the mouse input instance and the pointer to it
//param->mouseInputPointer->The pointer to the mouse input
void MouseInput::DestroyMouseInput(MouseInput* &mouseInputPointer)
{
	//if instance exists, delete it and set both pointers to null
	if(instance)
	{
		delete instance;
		instance = nullptr;
		mouseInputPointer = nullptr;
	}
}

//Gets the instance of this mouse input
MouseInput* MouseInput::GetInstance()
{
	if(instance == nullptr)
		instance = new MouseInput();

	return instance;
}

//Sets the previous states to the last frames' current states
void MouseInput::SetPreviousStates()
{
	//reset mouse moved
	mouseMoved = false;

	//Loop through the mouse button states and set them
	for(int i = 0; i < 5; i++)
	{
		previousMButtonStates[i] = currentMButtonStates[i];
	}
	
	//reset the scroll
	currentMouseScroll.x = 0;
	currentMouseScroll.y = 0;	
}

//Update the Mouse Input
//param:mouseEvent->The mouse event to poll
void MouseInput::Update(SDL_Event &mouseEvent)
{	
	//Set button states if the event is a button push
	if(mouseEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		currentMButtonStates[mouseEvent.button.button - 1] = true;
	}
	if(mouseEvent.type == SDL_MOUSEBUTTONUP)
	{
		currentMButtonStates[mouseEvent.button.button - 1] = false;
	}
	
	//If it's a mouse movement event
	if(mouseEvent.type == SDL_MOUSEMOTION)
	{
		//Store that the mouse has moved
		mouseMoved = true;

		//update positions
		currentRelativeMousePos.x = mouseEvent.motion.xrel;
		currentRelativeMousePos.y = mouseEvent.motion.yrel;
		
		currentMousePos.x = mouseEvent.motion.x;
		currentMousePos.y = mouseEvent.motion.y;
		
	}

	//If it's a mouse wheel event, get the new mouse wheel positions
	if(mouseEvent.type == SDL_MOUSEWHEEL)
	{
		currentMouseScroll.x = mouseEvent.wheel.x;
		currentMouseScroll.y = mouseEvent.wheel.y;
	}
}

//Checks if a mouse button is currently in a down state
//param:mouseButtonToCheck->The mouse button to evaluate. Use
//SDL_BUTTON_*mouseButton*
bool MouseInput::GetMouseButtonDown(Uint8 mouseButtonToCheck)
{
	//SDL 2.0 starts there button count at 1. To match it to the array, 
	//we must subtract 1.
	return currentMButtonStates[mouseButtonToCheck - 1];
}

//Checks if a mouse button is currently in an up state
//param:mouseButtonToCheck->The mouse button to evaluate. Use
//SDL_BUTTON_*mouseButton*
bool MouseInput::GetMouseButtonUp(Uint8 mouseButtonToCheck)
{
	//SDL 2.0 starts there button count at 1. To match it to the array, 
	//we must subtract 1.
	return !currentMButtonStates[mouseButtonToCheck - 1];
}

//Checks if a mouse button was pressed
//param:mouseButtonToCheck->The mouse button to evaluate. Use
//SDL_BUTTON_*mouseButton*
bool MouseInput::GetMouseButtonPress(Uint8 mouseButtonToCheck)
{
	//SDL 2.0 starts there button count at 1. To match it to the array, 
	//we must subtract 1.
	return (previousMButtonStates[mouseButtonToCheck - 1] == true) && 
		(!currentMButtonStates[mouseButtonToCheck-1] == false); 
}

//Check if the mouse has been moved
bool MouseInput::GetMouseMoved()
{
	return mouseMoved;
}

//Gets the current mouse position and returns it as an SDL_Point
//param:x->Pointer to value to store the x coordinate
//param:y->Pointer to value to store the y coordinate
//Returns an SDL_Point, even if x and y aren't null
SDL_Point MouseInput::GetMousePosition(int* x, int* y)
{
	if(x)
		*x = currentMousePos.x;
	if(y)
		*y = currentMousePos.y;

	return currentMousePos;
}

//Gets the relative mouse position (only filled when relative mode is enabled)
//param:x->The pointer to the value to place the x-coordinate into
//param:y->The pointer to the value to place the y-coordinate into
//Returns the relative mouse position as an SDL_Point, even if x and y aren't null
SDL_Point MouseInput::GetMouseRelativeModePosition(int* x, int* y)
{
	if(x)
		*x = currentRelativeMousePos.x;
	if(y)
		*y = currentRelativeMousePos.y;

	return currentRelativeMousePos;
}

//Gets the direction that the mouse was moved in (not normalized)
//param:x->Pointer to a float to set the x coordinate to
//param:y->Pointer to a float to set the y coordinate to
//returns an SDL_Point even if x and y aren't null
SDL_Point MouseInput::GetMouseDirection(int* x , int *y )
{
	if(x)
		*x = currentRelativeMousePos.x;
	if(y)
		*y = currentRelativeMousePos.y;

	return currentRelativeMousePos;
}

//Gets the amount that the wheel was scrolled in the two directions
//param:x->Pointer to an int to store the x coordinate
//param:y->Pointer to an int to store the y coordinate
//Returns an SDL_Point with the coordinates, even if x and y are null
SDL_Point MouseInput::GetMouseScroll(int* x, int* y)
{
	//if x or y isn't null, set them
	if(x)
		*x = currentMouseScroll.x;
	if(y)
		*y = currentMouseScroll.y;

	return currentMouseScroll;

}

//Sets RelativeMouseMode. Check SDL_Documentation for effects
//param:enabled->True to enable, false to disable
void MouseInput::SetRelativeMouseMode(bool enabled)
{
	SDL_SetRelativeMouseMode((SDL_bool)enabled);
}

//Sets the mouse position to the given coordinates of the window with current focus
//param:x->X coordinate of the window to set it to
//param:y->Y coordinate of the window to set it to
void MouseInput::SetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(NULL, x, y);
}

//Sets the mouse position to the given coordinates of the window with current focus
//param:point->The point containing the coordinates of the window to set the mouse to
void MouseInput::SetMousePosition(SDL_Point point)
{
	SetMousePosition(point.x, point.y);
}