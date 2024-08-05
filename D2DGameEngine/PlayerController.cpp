#include "PlayerController.h"
#include "InputComponent.h"

void PlayerController::SetupInputComponent()
{
	inputComponent = CreateDefaultSubobject<InputComponent>();
	components.push_back(inputComponent);
}
