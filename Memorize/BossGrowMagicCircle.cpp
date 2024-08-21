#include "BossGrowMagicCircle.h" 
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Boss.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/intersectionUtil.h"

#include "Player.h"

BossGrowMagicCircle::BossGrowMagicCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	bm->isVisible = false;
	abm->MarkBoundsDirty();

	CreateComponent<ClickComponent>();

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
	circleComponent->bSimulatePhysics = false;				// �����ӿ� ������ �����մϴ�.
	circleComponent->bApplyImpulseOnDamage = true;	// �������� ���� �� ����� ���մϴ�.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	circleComponent;	// ���� ������Ʈ�� ��Ʈ ������Ʈ�� �浹ü �Դϴ�.
	abm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowMagicCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowMagicCircleDamageEvent.origin = GetLocation();
	BossGrowMagicCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossGrowMagicCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowMagicCircleDamageEvent.componentHits.resize(1);

}

BossGrowMagicCircle::~BossGrowMagicCircle()
{}

void BossGrowMagicCircle::BeginPlay()
{
	__super::BeginPlay();


	{
		abm->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowMagicCircle.png");
		abm->SliceSpriteSheet(1200, 1200, 0, 0, 0, 0);
		abm->SetFrameDurations({ 0.05f });
		abm->FrameResize(82);
		abm->SetLoop(true);
		abm->Trigger(true);
	}

	circleComponent->InitCircleRadius(1200 / 2);
	//circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();


	disfellCommandCount = 4;
	CreateDisfellCommand();
}

void BossGrowMagicCircle::Update(float _dt)
{
	__super::Update(_dt);
	if (skillDuration > 0.f)
	{
		abm->Scale(1.0f + _dt * 0.1f, 1.0f + _dt * 0.1f);
		skillDuration -= _dt;
	}
	if (skillDuration < 0.f)
	{
		BossGrowMagicCircleDamageEvent.radialDamageInfo.innerRadius = circleComponent->GetScaledSphereRadius();
		BossGrowMagicCircleDamageEvent.radialDamageInfo.outerRadius = circleComponent->GetScaledSphereRadius();
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
		BossGrowMagicCircleDamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;
		

		bool hitRadius = intersectionUtil::BoundaryCircleBoxIntersect(
			circleComponent->CalculateBounds(circleComponent->GetWorldTransform()).GetCircle(),
			player->rootComponent->CalculateBounds(player->rootComponent->GetWorldTransform()).GetBox()
		);
		if (hitRadius)
			player->TakeDamage(damage, BossGrowMagicCircleDamageEvent, nullptr, this);

		// �ִϸ��̼� �ʱ�ȭ
		Boss* boss = GetWorld()->FindActorByType<Boss>();
		Animator* abm = boss->abm;
		AnimationState* IdleAnimationState = boss->IdleAnimationState;
		AnimationState* CastingAnimationState = boss->CastingAnimationState;
		if (abm->GetCurrentAnimationScene() == CastingAnimationState)
			abm->SetState(IdleAnimationState);

		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();
		Destroy();
	}
}

bool BossGrowMagicCircle::Destroy()
{
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);

	return __super::Destroy();
}

void BossGrowMagicCircle::DisfellOneCountAction()
{
	abm->Scale(0.9f, 0.9f);
}

void BossGrowMagicCircle::DisfellAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void BossGrowMagicCircle::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void BossGrowMagicCircle::OnClicked()
{
	__super::OnClicked();
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void BossGrowMagicCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern06_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void BossGrowMagicCircle::ReflectionOut() {}
