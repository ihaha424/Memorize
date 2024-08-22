#include "ScarecrowMagicCircle.h" 
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/intersectionUtil.h"

#include "Player.h"
#include "Arena.h"

#include "CreatePurificationEffect.h"

ScarecrowMagicCircle::ScarecrowMagicCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	bm->isVisible = false;

	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	abm->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowMagicCircle.png");
	abm->SliceSpriteSheet(1200, 1200, 0, 0, 0, 0);
	abm->SetFrameDurations({ 0.05f });
	abm->FrameResize(82);
	abm->SetLoop(true);
	abm->Trigger(true);
	abm->MarkBoundsDirty();

	circleBreak = CreateComponent<AnimationBitmapComponent>();
	rootComponent->AddChild(circleBreak);
	circleBreak->SetSprite(L"TestResource/Boss/MagicCircle/BossCircleBreak.png");
	circleBreak->SliceSpriteSheet(1500, 1500, 0, 0, 0, 0);
	circleBreak->SetFrameDurations({ 1.f / 12.f });
	circleBreak->SetLoop(false);
	circleBreak->Trigger(false);
	circleBreak->isVisible = false;


	shockwave = CreateComponent<BitmapComponent>();
	rootComponent->AddChild(shockwave);
	shockwave->SetSprite(L"TestResource/Boss/MagicCircleShockwave.png");
	shockwave->isVisible = false;

	CreateComponent<ClickComponent>();

	//circleComponent = CreateComponent<CircleComponent>();

	//circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	//circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	//circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	//circleComponent->bGenerateOverlapEvent = false;	// Overlap 이벤트를 발생시킵니다.
	//circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
	//abm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowMagicCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowMagicCircleDamageEvent.origin = GetLocation();
	BossGrowMagicCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossGrowMagicCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowMagicCircleDamageEvent.radialDamageInfo.damageFalloff = 0.f;
	BossGrowMagicCircleDamageEvent.componentHits.resize(1);

	player = GetWorld()->FindActorByType<Player>();

	disfellCommandCount = 8;
	CreateDisfellCommand();
}

ScarecrowMagicCircle::~ScarecrowMagicCircle()
{}

void ScarecrowMagicCircle::BeginPlay()
{
	__super::BeginPlay();

	//circleComponent->InitCircleRadius(1200 / 2);
	//circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);


}

void ScarecrowMagicCircle::Update(float _dt)
{
	__super::Update(_dt);

	if (shockwaving)
	{
		shockwaveScale += _dt * 1.5f;
		shockwave->SetScale(shockwaveScale, shockwaveScale * 0.7f);
		shockwave->SetOpacity(shockwaveOpacity / 8.f);
		shockwaveOpacity = (std::max)(shockwaveOpacity - _dt * 0.01f, 0.f);
		shockwaveTimer -= _dt;
		if (shockwaveTimer <= 0.f)
		{
			Destroy();
		}
		return;
	}

	if (destructing)
	{
		destructionTimer -= _dt;
		if (destructionTimer <= 0.f)
		{
			Destroy();
		}
		return;
	}

	if (skillDuration > 0.f)
	{
		abm->Scale(1.0f + _dt * 0.1f, 1.0f + _dt * 0.1f);
		skillDuration -= _dt;
	}
	if (skillDuration < 0.f)
	{
		BossGrowMagicCircleDamageEvent.radialDamageInfo.innerRadius = 0.f;
		BossGrowMagicCircleDamageEvent.radialDamageInfo.outerRadius = 10000.f;
		BossGrowMagicCircleDamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;

		GetWorld()->FindActorByType<Arena>()->earthquake = true;
		player->TakeDamage(damage * (rootComponent->bounds.GetBox().GetWidth() / 3261.f), BossGrowMagicCircleDamageEvent, nullptr, this);

		// 애니메이션 초기화

		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();

		this->abm->Trigger(false);
		this->abm->isVisible = false;

		shockwaving = true;
		shockwave->isVisible = true;
	}
}

bool ScarecrowMagicCircle::Destroy()
{
	return __super::Destroy();
}

void ScarecrowMagicCircle::DisfellOneCountAction()
{
	abm->Scale(0.9f, 0.9f);
}

void ScarecrowMagicCircle::DisfellAction()
{
	abm->Trigger(false);
	abm->isVisible = false;

	circleBreak->Trigger(true);
	circleBreak->isVisible = true;

	EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();

	destructing = true;
}

void ScarecrowMagicCircle::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void ScarecrowMagicCircle::OnClicked()
{
	__super::OnClicked();
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void ScarecrowMagicCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern06_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void ScarecrowMagicCircle::ReflectionOut() {}
