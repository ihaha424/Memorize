#include "Skill.h"
#include "Player.h"
#include "GPlayerController.h"

int Skill::wholeSkillLevel = 0;

Skill::Skill(Actor* _owner) : IComponent(_owner)
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

	//TODO
	//플레이어 마나 사용

}