#pragma once

#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"


class BossMeteo : public BossSkillActor
{
public:
	BossMeteo(class World* _world);
	virtual ~BossMeteo() {/*∆¯∆»¿Ã∆Â∆Æ ? */ };

	virtual void BeginPlay();
	virtual void Update(float _dt) override;

	// BossSkillActor¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	void ReflectionIn() override;
	void ReflectionOut() override;

	void SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _playerPos);

public:
	class AnimationBitmapComponent* abm;
	class AnimationBitmapComponent* Rangeabm;

private:
	RadialDamageEvent BossMeteoDamageEvent;
	class Player* player{ nullptr };

	float			 meteoPosX;
	DotTween<float>* meteoPosTweenX;
	float			 meteoPosY;
	DotTween<float>* meteoPosTweenY;
	
};

