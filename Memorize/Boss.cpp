#include "Boss.h"
#include "Signal.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "../D2DGameEngine/BoxComponent.h"

#include "BossHitEffect.h"

Boss::Boss(World* _world) : Character(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	//box->collisionProperty.SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Block);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Overlap);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Overlap);

	hp = maxHp * 0.1f;

	OnHPChanged = new Signal<float>;

	box->SetCollisionObjectType(ECollisionChannel::Enemy);
}

Boss::~Boss()
{
	delete OnHPChanged;
}

void Boss::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	OnHPChanged->Emit(hp/(float)maxHp);

	if (Periodic_Pattern_Cool_Time > 0.f)
		Periodic_Pattern_Cool_Time -= _dt;

	OBJ_MESSAGE(dbg::text(Periodic_Pattern_Cool_Time));
}

void Boss::OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult)
{
	OBJ_MESSAGE("Hit!");
}

void Boss::OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser)
{
	hp -= damageAmount;	// 체력을 받은 데미지 만큼 감소시킵니다.

	BossHitEffect* bosshitEffect = GetWorld()->GetEffectSystem().CreateEffect<BossHitEffect>();
	bosshitEffect->SetEffect(1);
	Math::Vector2 thisPos = GetLocation();
	bosshitEffect->SetLocation(thisPos.x, thisPos.y);
	if (hp <= 0.f)	// 만약 체력이 0보다 작거나 같다면,
	{
		hp = 0.f;
		//TODO 사망 시 처리 
	}
}