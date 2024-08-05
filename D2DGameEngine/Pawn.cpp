#include "Pawn.h"

Pawn::Pawn()
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

