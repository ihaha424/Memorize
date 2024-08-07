#pragma once
#include "../D2DGameEngine/Actor.h"

enum ESkillType
{
	ST_PROJECTILE, ST_RANGE, ST_BUFF, ST_SPECIAL
};

enum ESkillElement
{
	SE_FIRE, SE_WATER, SE_LIGHT, SE_DARKNESS
};


class Skill : public Actor
{
	static int wholeSkillLevel;

protected:
	class Player* player;
	std::wstring id;
	ESkillType type;
	int level = 0;
	int count = 0;
	int mana = 0;
	
public:
	Skill(class World* _world, ESkillType _type, std::wstring _id);
	virtual ~Skill();

	static int GetWholeSkillLevel() { return wholeSkillLevel; }

	/**
	 * @brief 스킬을 사용합니다. 스킬 사용 카운트가 증가합니다. 
	 */
	virtual void UseSkill();


	virtual void Update(float _dt) override;

};

