#include "MPMaxIncrease.h"

MPMaxIncrease::MPMaxIncrease(Actor* _owner) : BuffSkill(_owner)
{
	buffStat = Stat(0.f, 0.f, 0.f, additionalMP);
}

MPMaxIncrease::~MPMaxIncrease()
{
}

void MPMaxIncrease::UseSkill()
{
	player->AddToStat(buffStat);
}
