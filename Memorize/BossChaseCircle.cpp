#include "BossChaseCircle.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"

#include "D2DGameEngine/AnimationEffect.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Boss.h"

#include "D2DGameEngine/World.h"
#include "Player.h"

BossChaseCircle::BossChaseCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();
	disfellCommandCount = 2;
	CreateDisfellCommand();
	CreateComponent<ClickComponent>();

	bm->isVisible = false;

	circleComponent = CreateComponent<CircleComponent>();
	rootComponent = circleComponent;
	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = true;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap 이벤트를 발생시킵니다.
	circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
	circleComponent->maxSpeed = 800.f;

	magicCircle = CreateComponent<AnimationBitmapComponent>();
	rootComponent->AddChild(magicCircle);
	magicCircle->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowMagicCircle.png");
	magicCircle->SliceSpriteSheet(1200, 1200, 0, 0, 0, 0);
	magicCircle->SetFrameDurations({ 1.f / 12.f });
	magicCircle->FrameResize(82);
	magicCircle->SetLoop(true);
	magicCircle->Trigger(true);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossChaseCircleDamageEvent.SetDamageType(radiaDamageType);
	BossChaseCircleDamageEvent.origin = GetLocation();
	BossChaseCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossChaseCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossChaseCircleDamageEvent.componentHits.resize(1);

	for (int i = 0; i < disfellCommand.size(); i++)
		std::cout << (int)disfellCommand[i] + 1 << ", ";

	speedTween = new DotTween<float>(&speedVarias, EasingEffect::OutQuad, StepAnimation::StepOnceForward);
	speedTween->SetDuration(5.f);
	speedTween->SetStartPoint(-100.f);
	speedTween->SetEndPoint(400.f);

	scaleTween = new DotTween<float>(&scaleVarias, EasingEffect::OutQuad, StepAnimation::StepOnceForward);
	scaleTween->SetDuration(5.f);
	scaleTween->SetStartPoint(0.9f);
	scaleTween->SetEndPoint(1.5f);

	circleComponent->InitCircleRadius(magicCircle->GetFrameHeight() / 2.f);	// 반지름이 62이고 높이가 110 인 캡슐 충돌체를 초기화 합니다.
	circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

BossChaseCircle::~BossChaseCircle()
{}

void BossChaseCircle::BeginPlay()
{
	__super::BeginPlay();
	
}

void BossChaseCircle::Update(float _dt)
{
	__super::Update(_dt);

	skillDuration -= _dt;
	if (skillDuration > 0.f)
	{
		if (!ignited)
		{
			LOG_MESSAGE(dbg::text(GetLocation().x, ", ", GetLocation().y));
			Math::Vector2 toPlayer = player->GetLocation() - GetLocation();
			if (toPlayer.Length() < 30.f)
				return;
			toPlayer.Normalize();
			speedTween->Update(_dt);

			scaleTween->Update(_dt);

			SetScale(scaleVarias * 0.3f, scaleVarias * 0.3f);
			SetVelocity(toPlayer * (speed + speedVarias));

			if (skillDuration <= 1.f)
			{
				explosionTimer -= _dt;
				ignited = true;
			}
		}
		else
		{
			magicCircle->SetFrameDurations({ 1.f / (12.f * 5.f) });
		}
	}
	if (skillDuration < 0.f)
	{
		BossChaseCircleDamageEvent.radialDamageInfo.innerRadius = 500.f;
		BossChaseCircleDamageEvent.radialDamageInfo.outerRadius = 500.f;
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
		BossChaseCircleDamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;

		Box playerBox = Box::BuildAABB({ 0.f, 0.f }, player->collisionBox->GetBoxExtent());
		playerBox.ul = DXVec2::Transform(playerBox.ul, player->collisionBox->GetWorldTransform());
		playerBox.lr = DXVec2::Transform(playerBox.lr, player->collisionBox->GetWorldTransform());

		bool hitRadius = intersectionUtil::BoxCircleIntersect(
			playerBox,
			circleComponent->CalculateBounds(circleComponent->GetWorldTransform()).GetCircle()
		);
		if (hitRadius)
			player->TakeDamage(damage, BossChaseCircleDamageEvent, nullptr, this);

		// 애니메이션 초기화
		Boss* boss = GetWorld()->FindActorByType<Boss>();
		Animator* abm = boss->abm;
		AnimationState* IdleAnimationState = boss->IdleAnimationState;
		AnimationState* CastingAnimationState = boss->CastingAnimationState;
		if (abm->GetCurrentAnimationScene() == CastingAnimationState)
			abm->SetState(IdleAnimationState);

		// 이펙트
		AnimationEffect* DestoryProjectileEffect = GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		DestoryProjectileEffect->SetSprite(L"TestResource/Boss/Boss_Meteor/BOSS_Skill_Meteor_Explosion.png");
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetScale(1.4f, 1.4f);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 0.5f / 36.f });
		DestoryProjectileEffect->GetAnimationBitmapComponent()->FrameSplice(6, DestoryProjectileEffect->GetAnimationBitmapComponent()->GetFrameSize());
		DestoryProjectileEffect->GetAnimationBitmapComponent()->Trigger(true);
		DestoryProjectileEffect->SetAliveTime(.5f);

		auto Pos = GetLocation();
		DestoryProjectileEffect->SetLocation(Pos.x, Pos.y);

		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();

		Destroy();
	}
}

bool BossChaseCircle::Destroy()
{
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);

	return __super::Destroy();
}

void BossChaseCircle::DisfellAction()
{
	// 애니메이션 초기화
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);

	Destroy();
}

void BossChaseCircle::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void BossChaseCircle::OnClicked()
{
	__super::OnClicked();
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void BossChaseCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern11_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage, speed, duration);
	type = static_cast<BossSkillType>(typeInt);
}

void BossChaseCircle::ReflectionOut()
{
}
