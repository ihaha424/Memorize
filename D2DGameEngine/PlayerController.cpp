#include "PlayerController.h"
#include "InputComponent.h"

PlayerController::PlayerController(World* _world) : Controller(_world)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::SetupInputComponent()
{
	inputComponent = CreateComponent<InputComponent>();
	components.insert({ std::type_index(typeid(InputComponent)), inputComponent });
}
