#pragma once
#include "D2DGameEngine/Actor.h"

class ElementalExplosionEffect : public Actor
{
	class BitmapComponent* bm;
	float duration = 0.5f;
	float elapsedTime = 0.f;
	float damage = 0.f;
	class BoxComponent* box;
public:
	ElementalExplosionEffect(World* _world);
	virtual ~ElementalExplosionEffect();

	void OnBeginOverlap(Actor* other, const OverlapInfo& overlap);

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;
};

//Skill_Mana: 100
//Command : 3, 2, 2, 0, 0, 2
//Skill_Condition : 30
//Castring Time : 0.5
// bCrash : 1
//Skill_Text : 모든 투사체를 폭발시킵니다.
//End