#include "Skill.h"
#include "Player.h"
#include "GPlayerController.h"

int Skill::wholeSkillLevel = 0;

Skill::Skill()
{
}

Skill::~Skill()
{
}

void Skill::SetPlayer(GPlayerController* _pc)
{
	controller = _pc;
	player = controller->GetPlayer();
}

void Skill::UseSkill()
{
	count++;

	//TODO
	//플레이어 마나 사용

}

void Skill::Update(float _dt)
{
	//5회 사용 시 다음 레벨이 된다.
	if (count >= 5)
	{
		count = 0;
		level++;
		wholeSkillLevel++;
	}
}
