#include "GamepadInput.h"

static GamepadInput* instance;

GamepadInput::GamepadInput(void)
{
}


GamepadInput::~GamepadInput(void)
{
}

//Destroys the instance of the gamepad and sets the pointer to null
//param:pointerToInstance->The pointer to this gamePad input to set to null
void GamepadInput::DestroyGamepadInstance(GamepadInput* &pointerToInstance)
{
	if(instance)
	{
		delete instance;
		instance = nullptr;
		pointerToInstance = nullptr;
	}
}

//Gets the instance of the gamepad Input
GamepadInput* GamepadInput::GetInstance()
{
	if(instance == nullptr)
		instance = new GamepadInput();

	return instance;
}

void GamepadInput::Update(SDL_Event &gamePadEvent)
{
	
}