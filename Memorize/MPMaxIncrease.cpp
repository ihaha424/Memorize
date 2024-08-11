#include "MPMaxIncrease.h"

MPMaxIncrease::MPMaxIncrease(Actor* _owner) : BuffSkill(_owner)
{
	SetID(ST_BUFF, SE_LIGHT);
	buffStat = Stat(0.f, 0.f, 0.f, additionalMP);
}

MPMaxIncrease::~MPMaxIncrease()
{
}

void MPMaxIncrease::UseSkill()
{
	player->AddToStat(buffStat);
}
