#include "PlayerController.h"
#include "InputComponent.h"

void PlayerController::SetupInputComponent()
{
	inputComponent = CreateDefaultSubobject<InputComponent>();
	components.insert({ std::type_index(typeid(InputComponent)), inputComponent });
}
