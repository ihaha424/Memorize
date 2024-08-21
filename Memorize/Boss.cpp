#include "Boss.h"
#include "Signal.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "../D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Player.h"
#include "SkillActor.h"
#include "BossHitEffect.h"
#include "D2DGameEngine/RandomGenerator.h"

Boss::Boss(World* _world) : Character(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	renderLayer = 3;

	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	//box->collisionProperty.SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Block);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Overlap);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Overlap);
	box->SetBoxExtent({ 187 / 2.8f, 287 / 2.4f });
	hp = 1;

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

			CastingAnimationState = abm->CreateState<AnimationState>();
			CastingAnimationState->SetSprite(L"TestResource/Boss/BossMotions/Boss_Casting.png");
			CastingAnimationState->SliceSpriteSheet(149, 280, 0, 0, 0, 0);
			CastingAnimationState->SetFrameDurations({ 1.f / 12.f });
			CastingAnimationState->FrameResize(9);
			CastingAnimationState->Trigger(true);

			DieAnimationState = abm->CreateState<AnimationState>();
			DieAnimationState->SetSprite(L"TestResource/Boss/BossMotions/Boss_Dead.png");
			DieAnimationState->SliceSpriteSheet(250, 280, 0, 0, 0, 0);
			DieAnimationState->SetFrameDurations({ 1.f / 12.f });
			DieAnimationState->FrameResize(33);
			DieAnimationState->SetLoop(false);
			DieAnimationState->Trigger(true);

			TeleportStartAnimationState = abm->CreateState<AnimationState>();
			TeleportStartAnimationState->SetSprite(L"TestResource/Boss/BossMotions/Boss_Teleport_Start.png");
			TeleportStartAnimationState->SliceSpriteSheet(300, 300, 0, 0, 0, 0);
			TeleportStartAnimationState->SetFrameDurations({ 1.f / 12.f });
			TeleportStartAnimationState->Trigger(true);

			TeleportEndAnimationState = abm->CreateState<AnimationState>();
			TeleportEndAnimationState->SetSprite(L"TestResource/Boss/BossMotions/Boss_Teleport_End.png");
			TeleportEndAnimationState->SliceSpriteSheet(300, 300, 0, 0, 0, 0);
			TeleportEndAnimationState->SetFrameDurations({ 1.f / 12.f });
			TeleportEndAnimationState->Trigger(true);
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
	if (hp > 0.f)
	{
		Math::Vector2 velocity = GetVelocity();
		if (velocity.Length() < 10.f)
		{
			if (abm->GetCurrentAnimationScene() == MoveAnimationState)
				abm->SetState(IdleAnimationState);
		}
		else
		{
			if (abm->GetCurrentAnimationScene() == IdleAnimationState)
				abm->SetState(MoveAnimationState);
		}
	}
	else
	{
		if (abm->GetCurrentAnimationScene() != DieAnimationState)
			abm->SetState(DieAnimationState);
	}
}

void Boss::OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult)
{
	OBJ_MESSAGE("Hit!");
}

void Boss::OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser)
{
	hp -= damageAmount;	// 체력을 받은 데미지 만큼 감소시킵니다.

	SkillActor* temp = static_cast<SkillActor*>(damageCauser);
	BossHitEffect* bosshitEffect = GetWorld()->GetEffectSystem().CreateEffect<BossHitEffect>();
	bosshitEffect->SetEffect(temp->skill.element);
	Math::Vector2 thisPos = GetLocation();
	bosshitEffect->SetLocation(thisPos.x + Random::Get((int)200) - 100, thisPos.y + Random::Get((int)500) - 250);
	if (hp <= 0.f)	// 만약 체력이 0보다 작거나 같다면,
	{
		hp = 0.f;
		//TODO 사망 시 처리 
	}
}