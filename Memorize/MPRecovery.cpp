#include "MPRecovery.h"

MPRecovery::MPRecovery(Actor* _owner) : BuffSkill(_owner)
{
	buffStat =
	{
		0.f,
		0.f,
		0.f,
		0.f,
		0.f,
		mpRecovery,
		0.f,
		0,
		0,
		0
	};
}

MPRecovery::~MPRecovery()
{
}
