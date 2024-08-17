#pragma once
#include "Pawn.h"
class Character : public Pawn
{
public:
	Character(class World* _world);
	virtual ~Character();


	class BoxComponent* box;

};

