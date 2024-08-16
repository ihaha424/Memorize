#pragma once

#include "../D2DGameEngine/Event.h"
#include "BossSkillActor.h"

struct DisFellEvent : public Event<DisFellEvent> {
	DisFellEvent(BossSkillActor* _target) :target( std::make_shared<BossSkillActor*>(_target))
	{}
		
	void SetBossSkillActor(BossSkillActor* _damageType) 
	{
		target = std::make_shared<BossSkillActor*>(_damageType);
	}
	BossSkillActor* GetBossSkillActor() const
	{
		return *target.get();
	}

private:
	std::shared_ptr<BossSkillActor*> target;
};