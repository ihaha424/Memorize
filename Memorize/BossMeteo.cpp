#include "BossMeteo.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "Player.h"

#include "../D2DGameEngine/AnimationEffect.h"


BossMeteo::BossMeteo(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	bm->isVisible = false;
	rootComponent = bm;
	Rangeabm = CreateComponent<AnimationBitmapComponent>();
	bm->AddChild(Rangeabm);

	abm = CreateComponent<AnimationBitmapComponent>();
	bm->AddChild(abm);

	DamageType radiaDamageType{
	.damageImpulse = 10000.f,
	};
	BossMeteoDamageEvent.SetDamageType(radiaDamageType);
	BossMeteoDamageEvent.radialDamageInfo.maxDamage = damage;
	BossMeteoDamageEvent.radialDamageInfo.minDamage = damage;
	BossMeteoDamageEvent.radialDamageInfo.innerRadius = 100.f;
	BossMeteoDamageEvent.radialDamageInfo.outerRadius = 100.f;
	BossMeteoDamageEvent.componentHits.resize(1);

	meteoPosTweenX = new DotTween<float>(&meteoPosX, EasingEffect::InQuart, StepAnimation::StepOnceForward);
	meteoPosTweenX->SetDuration(time);
	meteoPosTweenY = new DotTween<float>(&meteoPosY, EasingEffect::InQuart, StepAnimation::StepOnceForward);
	meteoPosTweenY->SetDuration(time);
}

void BossMeteo::BeginPlay()
{
	__super::BeginPlay();

	{
		Rangeabm->SetSprite(L"TestResource/Boss/MagicCircle/BossMetemoRange.png");
		Rangeabm->SliceSpriteSheet(250, 330, 0, 0, 0, 0);
		Rangeabm->FrameResize(20);
		Rangeabm->SetFrameDurations({ 0.05f });
		Rangeabm->SetLoop(true);
		Rangeabm->Trigger(true);
	}

	{
		abm->SetSprite(L"TestResource/Boss/Boss_Meteor/BOSS_Skill_Meteor_Start.png");
		abm->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
		abm->FrameResize(73);
		abm->SetFrameDurations({ 0.05f });
		abm->SetLoop(true);
		abm->Trigger(true);
	}

	player = GetWorld()->FindActorByType<Player>();
}

void BossMeteo::Update(float _dt)
{
	__super::Update(_dt);

	time -= _dt;
	meteoPosTweenX->Update(_dt);
	meteoPosTweenY->Update(_dt);
	abm->SetTranslation(meteoPosX, meteoPosY);
	if (time < 0.f)
	{
		bool hitRadius = intersectionUtil::BoundaryCircleBoxIntersect(
			Circle(BossMeteoDamageEvent.origin, BossMeteoDamageEvent.radialDamageInfo.innerRadius),
			player->rootComponent->CalculateBounds(player->GetTrasnform()).GetBox());
		if(hitRadius)
			player->TakeDamage(damage, BossMeteoDamageEvent, nullptr, this);
		Destroy();
	}
}

bool BossMeteo::Destroy()
{
	//BlickSource Effect
	{
		AnimationEffect* DestoryProjectileEffect = GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		DestoryProjectileEffect->SetSprite(L"TestResource/Boss/Boss_Meteor/BOSS_Skill_Meteor_Explosion.png");
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 0.040f });
		DestoryProjectileEffect->GetAnimationBitmapComponent()->Trigger(true);
		DestoryProjectileEffect->SetAliveTime(1.5f);

		auto Pos = Rangeabm->GetComponentLocation();
		DestoryProjectileEffect->SetLocation(Pos.x, Pos.y);
	}
	return __super::Destroy();
}


void BossMeteo::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern13_Actor.txt");
	reflectionResource->ParsingFile(0, isDispel, dispelTime, skillDuration, damage);
}

void BossMeteo::ReflectionOut()
{}

void BossMeteo::SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _playerPos)
{
	BossMeteoDamageEvent.origin = _playerPos;

	Rangeabm->SetTranslation(_playerPos.x, _playerPos.y);
	meteoPosTweenX->SetStartPoint(_startPos.x);
	meteoPosTweenX->SetEndPoint(_playerPos.x);
	meteoPosTweenY->SetStartPoint(_startPos.y);
	meteoPosTweenY->SetEndPoint(_playerPos.y);

	Math::Vector2 w = _playerPos - _startPos; w.Normalize();
	Math::Vector2 v = Up();

	float x = w.y * v.x - w.x * v.y;
	float y = w.x * v.x + w.y * v.y;
	float rad = atan2(x, y);

	abm->Rotate(Math::RadianToDegree(rad));
}
