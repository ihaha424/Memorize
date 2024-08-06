#include "PlayerController.h"
#include "InputComponent.h"

void PlayerController::SetupInputComponent()
{
	inputComponent = CreateComponent<InputComponent>();
	components.insert({ std::type_index(typeid(InputComponent)), inputComponent });
}
