#pragma once
#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "../D2DGameEngine/ReflectionResource.h"

class BossProjectile : public BossSkillActor
{
public:
	BossProjectile(class World* _world);
	virtual ~BossProjectile() {};

	virtual void BeginPlay();

	virtual void Update(float _dt) override;
	virtual bool Destroy() override;
	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	void SetActor(std::wstring _fileName);

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

	void SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _direction);

public:
	class CircleComponent* circleComponent;
	class AnimationBitmapComponent* abm;

private:
	std::shared_ptr<ReflectionResource> reflectionResource;
	RadialDamageEvent BossProjectileDamageEvent;
	class Player* player{ nullptr };
};

