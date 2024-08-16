#pragma once

#include "../D2DGameEngine/Event.h"
#include "BossSkillActor.h"

struct DisFellEvent : public Event<DisFellEvent> {
	DisFellEvent(BossSkillActor* _target, bool _dieFlag) :target( std::make_shared<BossSkillActor*>(_target)), dieFlag(_dieFlag)
	{}
		
	void SetBossSkillActor(BossSkillActor* _damageType, bool _dieFlag) 
	{
		target = std::make_shared<BossSkillActor*>(_damageType);
		dieFlag = _dieFlag;
	}
	BossSkillActor* GetBossSkillActor() const
	{
		return *target.get();
	}
	bool GetBossSkillDieFlag() const
	{
		return dieFlag;
	}

private:
	std::shared_ptr<BossSkillActor*> target;
	bool	dieFlag;
};