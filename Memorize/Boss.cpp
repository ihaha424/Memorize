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
	hp -= damageAmount;	// ü���� ���� ������ ��ŭ ���ҽ�ŵ�ϴ�.

	BossHitEffect* bosshitEffect = GetWorld()->GetEffectSystem().CreateEffect<BossHitEffect>();
	bosshitEffect->SetEffect(1);
	Math::Vector2 thisPos = GetLocation();
	bosshitEffect->SetLocation(thisPos.x, thisPos.y);
	if (hp <= 0.f)	// ���� ü���� 0���� �۰ų� ���ٸ�,
	{
		hp = 0.f;
		//TODO ��� �� ó�� 
	}
}