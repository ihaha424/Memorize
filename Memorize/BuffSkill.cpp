#include "BuffSkill.h"

BuffSkill::BuffSkill(World* _world, std::wstring _id)
	: Skill(_world, ST_BUFF, _id)
{
}

BuffSkill::~BuffSkill()
{
}

void BuffSkill::UseSkill()
{
	__super::UseSkill();

	//TODO
	//Ω∫≈»¿ª ø√∑¡¡‹
}
