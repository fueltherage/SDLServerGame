#pragma once
#include <SDL.h>

class GamepadInput
{
public:
	//Gets the instance of this gamepad input singleton
	static GamepadInput* GetInstance();

	~GamepadInput(void);

	//Destroys the instance of the gamepad and sets the pointer to null
	//param:pointerToInstance->The pointer to this gamePad input to set to null
	void DestroyGamepadInstance(GamepadInput* &pointerToInstance);

	void Update(SDL_Event &gamePadEvent);

private:

GamepadInput(void);
};

