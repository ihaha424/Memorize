#include "Boss.h"
#include "Signal.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "../D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Player.h"

#include "BossHitEffect.h"

Boss::Boss(World* _world) : Character(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	//box->collisionProperty.SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Block);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Overlap);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Overlap);

	hp = maxHp;

	OnHPChanged = new Signal<float>;

	box->SetCollisionObjectType(ECollisionChannel::Enemy);


	// 애니메이션
	{
		abm = CreateComponent<Animator>();
		rootComponent->AddChild(abm);
		{
			IdleAnimationState = abm->CreateState<AnimationState>();
			IdleAnimationState->SetSprite(L"TestResource/Boss/BossMotions/Boss_Idle.png");
			IdleAnimationState->SliceSpriteSheet(187, 287, 0, 0, 0, 0);
			IdleAnimationState->SetFrameDurations({ 0.06f });
			IdleAnimationState->Trigger(true);
			abm->Initialize(IdleAnimationState);

			MoveAnimationState = abm->CreateState<AnimationState>();
			MoveAnimationState->SetSprite(L"TestResource/Boss/BossMotions/Boss_Moving.png");
			MoveAnimationState->SliceSpriteSheet(187, 280, 0, 0, 0, 0);
			MoveAnimationState->SetFrameDurations({ 0.06f });
			MoveAnimationState->Trigger(true);
		}
	}

	SetScale(2.f, 2.f);
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

	//Flip
	Math::Vector2 playerPos = GetWorld()->FindActorByType<Player>()->GetLocation();
	playerPos = GetLocation() - playerPos;
	if (playerPos.x < 0.f)
		abm->SetScale(-1.f, 1.f);
	else
		abm->SetScale(1.f, 1.f);

	//Animation
	Math::Vector2 velocity = GetVelocity();
	if (velocity.Length() < 10.f)
	{
		if (abm->GetCurrentAnimationScene() != IdleAnimationState)
			abm->SetState(IdleAnimationState);
	}
	else
	{
		if (abm->GetCurrentAnimationScene() != MoveAnimationState)
			abm->SetState(MoveAnimationState);
	}
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