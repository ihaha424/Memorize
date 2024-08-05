#include "Pawn.h"

Pawn::Pawn()
{
}

Pawn::~Pawn()
{
}

void Pawn::PossessedBy(Controller* _controller)
{
	controller = _controller;
}

void Pawn::SetActorLocation(Math::Vector2 location)
{
	if (rootComponent == nullptr)
		return;

	//TODO
}

Math::Vector2 Pawn::GetActorLocation()
{
	//TODO
	return Math::Vector2();
}
