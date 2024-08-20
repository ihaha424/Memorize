#include "Pawn.h"
#include "Controller.h"

Pawn::Pawn(class World* _world) : Actor(_world)
{
}

Pawn::~Pawn()
{
}

void Pawn::SetController(Controller* _controller)
{
	controller = _controller;
	controller->SetPawn(this);
}

