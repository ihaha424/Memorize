#include "Pawn.h"
#include "Controller.h"

Pawn::Pawn(class World* _world) : Actor(_world)
{
}

Pawn::~Pawn()
{
	if (controller)
	{
		delete controller;
		controller = nullptr;
	}
}

void Pawn::SetController(Controller* _controller)
{
	controller = _controller;
	controller->SetPawn(this);
}

