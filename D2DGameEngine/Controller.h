#pragma once
#include "Actor.h"

class Controller : public Actor
{
protected:
	class Pawn* owner;
public:
	Controller(class World* _world);
	virtual ~Controller();
	void SetPawn(class Pawn* _pawn) { owner = _pawn; }
	class Pawn* GetPawn() { return owner; }
};

