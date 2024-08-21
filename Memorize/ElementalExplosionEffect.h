#pragma once
#include "SkillActor.h"

class ElementalExplosionEffect : public SkillActor
{
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
//Skill_Text : ��� ����ü�� ���߽�ŵ�ϴ�.
//End