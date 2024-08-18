#pragma once
#include "D2DGameEngine/IComponent.h"

class ElementalMasterComponent : public IComponent
{
	float nowSkillTimer = 0.f;
	class GPlayerController* controller;
	bool bSkillReady = false;
	float skillDuration = 0.f;
public:
	ElementalMasterComponent(Actor* _owner);
	virtual ~ElementalMasterComponent();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;
;
};

