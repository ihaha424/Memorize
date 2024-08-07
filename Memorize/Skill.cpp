#include "Skill.h"
#include "Player.h"

int Skill::wholeSkillLevel = 0;

Skill::Skill(World* _world, ESkillType _type, std::wstring _id) : Actor(_world)
{
	type = _type;
	id = _id;
}

Skill::~Skill()
{
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
