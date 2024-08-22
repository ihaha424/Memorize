#pragma once
#include "D2DGameEngine/IComponent.h"

class ElementalMasterComponent : public IComponent
{
	class GPlayerController* controller;
	bool bSkillReady = false;
	class BuffEffectComponent* abm;
	float elapsedTime = 0.f;
	bool bStartedLockAnim = false;
	class AnimationEffect* lockedEffect;
public:


	ElementalMasterComponent(Actor* _owner);
	virtual ~ElementalMasterComponent();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	void Initialize(class Player* player);
};

