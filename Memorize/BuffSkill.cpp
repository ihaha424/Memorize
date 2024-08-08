#include "BuffSkill.h"
#include "GPlayerController.h"

BuffSkill::BuffSkill()
{
	type = ST_BUFF;
}

BuffSkill::~BuffSkill()
{
}

void BuffSkill::Update(float _dt)
{
	elapsedTime += _dt;

	if (elapsedTime > buffDuration)
	{
		player->AddToStat(-buffStat);
		controller->EndSkill();
	}
}


void BuffSkill::UseSkill()
{
	__super::UseSkill();
	elapsedTime = 0.f;
	player->AddToStat(buffStat);
}
