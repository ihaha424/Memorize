#include "BuffSkill.h"
#include "GPlayerController.h"
#include "Player.h"
#include "D2DGameEngine/Animator.h"

BuffSkill::BuffSkill(Actor* _owner) : Skill(_owner)
{
	type = ST_BUFF;
}

BuffSkill::~BuffSkill()
{
}

void BuffSkill::UseSkill()
{
	__super::UseSkill();
	player->AddToStat(buffStat);
}
