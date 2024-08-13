#pragma once

#include <type_traits>

class IBossSkill
{
	enum MagicRingPos {BossFloor, BossDirection, PlayerFloor};
protected:
	MagicRingPos	posCircle = BossFloor;
	//Animation??

public:
	virtual void UseSkill() = 0;
};

template<class T>
concept  BossSkillType = std::is_base_of<IBossSkill, T>::value;
