#include "LightStreamEffect.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/DamageEvent.h"

LightStreamEffect::LightStreamEffect(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	rootComponent = bm = CreateComponent<BitmapComponent>();
	bm->SetSprite(L"TestResource/Skill/Range/LightStream.png");

	box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	box->bSimulatePhysics = false;	// 움직임에 물리를 적용하지 않습니다.
	box->bApplyImpulseOnDamage = false;	// 데미지를 받을 때 충격을 가합니다.
	box->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	rootComponent->AddChild(box);
}
void LightStreamEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void LightStreamEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	if (box->bGenerateOverlapEvent == false)
		return;

	//대미지를 입힘
	DamageEvent damageEvent;
	DamageType damageType{
		.damageImpulse = 10000.f, //충격량 넣어주는 게 맞는지?
	};
	damageEvent.SetDamageType(damageType);

	other->TakeDamage(damage, damageEvent, nullptr, this);

	//투과되지 않는 발세체의 경우 멈추고 폭발 처리
	if (!bIsPassable)
	{
		bEnding = true;
		anim->SetState(endingState);
		mv->SetSpeed(0);
		elapsedTime = duration + delay;
	}

	//다른 공격과 충돌하는 스킬은 충돌 처리
	if (bCollideWithOtherAttack)
	{
		BossProjectile* bossProjectile = dynamic_cast<BossProjectile*>(other);
		if (bossProjectile != nullptr)
		{
			bEnding = true;
			anim->SetState(endingState);
			mv->SetSpeed(0);
		}
	}
}

void LightStreamEffect::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;
	if (elapsedTime > duration)
	{
		elapsedTime = 0.f;
		Inactivate();
	}
}

LightStreamEffect::~LightStreamEffect()
{
}
