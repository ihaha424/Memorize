#pragma once
#include "Actor.h"
class Controller : public Actor
{
	class Pawn* ownerPawn;
public:
	Controller();
	virtual ~Controller();

	void SetPawn(Pawn* _pawn) { ownerPawn = _pawn; }

};

