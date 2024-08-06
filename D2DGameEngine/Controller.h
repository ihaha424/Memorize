#pragma once
#include "Actor.h"
class Controller : public Actor
{
protected:
	class Pawn* owner;
public:
	Controller();
	virtual ~Controller();

	void SetPawn(Pawn* _pawn) { owner = _pawn; }

};

