#include "BossRazer.h"

#include "D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"
#include "D2DGameEngine/ClickComponent.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Boss.h"

#include "Player.h"


BossRazer::BossRazer(World* _world) : BossSkillActor(_world)
{
	renderLayer = 2;

	bm->isVisible = false;

	isDispel = true;
	type = Range;
	dispelTime = 7.f;
	skillDuration = 2.f;
	// 스킬 시전 타이밍은 std::max(DispelTime - SkillDuration, 0);
	damage = 17.f;

	// cast time
	castTime = dispelTime - skillDuration;

	magicCircleHub = CreateComponent<BitmapComponent>();
	rootComponent = magicCircleHub;
	magicCircleHub->SetFrame({ 0,0 }, { 60, 240 });
	magicCircleHub->MarkBoundsDirty();
	magicCircleHub->isVisible = false;

	magicCircle = CreateComponent<AnimationBitmapComponent>();
	magicCircleHub->AddChild(magicCircle);
	magicCircle->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowMagicCircle.png");
	magicCircle->SliceSpriteSheet(1200, 1200, 0, 0, 0, 0);
	magicCircle->SetFrameDurations({ 1.f / 12.f });
	magicCircle->FrameResize(82);
	magicCircle->Trigger(true);
	magicCircle->SetLoop(true);
	magicCircle->Scale(0.05f, 0.2f);

	// TODO: 레이져 위치 가운데로 옮기기.
	lazerCharging = CreateComponent<AnimationBitmapComponent>();
	lazerCharging->SetSprite(L"TestResource/Boss/BossLazer/Boss_Laser1.png");
	lazerCharging->SetLoop(false);
	lazerCharging->SliceSpriteSheet(1920, 290, 0, 0, 0, 0);
	lazerCharging->SetFrameDurations({ 2.f / 25.f });
	lazerCharging->FrameResize(25);
	lazerCharging->Trigger(false);
	lazerCharging->isVisible = false;
	rootComponent->AddChild(lazerCharging);
	lazerCharging->SetScale(-1.f, 1.f);
	lazerCharging->Translate(-900.f, 0.f);

	// TODO: 레이져 위치 가운데로 옮기기.
	lazerShuttingDown = CreateComponent<AnimationBitmapComponent>();
	lazerShuttingDown->SetSprite(L"TestResource/Boss/BossLazer/Boss_Laser3.png");
	lazerShuttingDown->SetLoop(false);
	lazerShuttingDown->SliceSpriteSheet(1920, 290, 0, 0, 0, 0);
	lazerShuttingDown->SetFrameDurations({ 1.f / 19.f });
	lazerShuttingDown->FrameResize(19);
	lazerShuttingDown->Trigger(false);
	lazerShuttingDown->isVisible = false;
	rootComponent->AddChild(lazerShuttingDown);
	lazerShuttingDown->SetScale(-1.f, 1.f);
	lazerShuttingDown->Translate(-900.f, 0.f);

	lazer = CreateComponent<AnimationBitmapComponent>();
	lazer->SetSprite(L"TestResource/Boss/BossLazer/Boss_Laser2.png");
	lazer->SetLoop(true);
	lazer->SliceSpriteSheet(1920, 290, 0, 0, 0, 0);
	lazer->SetFrameDurations({ 5.f / 60.f });	// 12 frames per sec
	lazer->FrameResize(60);
	lazer->Trigger(false);		// -> Resume
	lazer->isVisible = false;	// -> True
	rootComponent->AddChild(lazer);
	obb = CreateComponent<PolygonComponent>();
	obb->InitPolygon({ {-960, -80}, {960, -80}, {960, 80}, {-960, 80} });
	lazer->AddChild(obb);
	obb->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);
	obb->bApplyImpulseOnDamage = true;
	obb->bGenerateOverlapEvent = false;	// -> True
	lazer->SetScale(-1.f, 1.f);
	lazer->Translate(-900.f, 0.f);

	// Damage Event
	DamageType damageType {
		.bCausedByWorld = false,
		.damageType = EDamageType::Default,
		.damageFalloff = 0.f,
		.damageImpulse = 10.f
	};
	RadialDamageInfo damageInfo(
		damageType,
		damage,
		damage,
		0.f,
		0.f,
		4000.f
	);
	razerDamageEvent.radialDamageInfo = damageInfo;

	disfellCommandCount = 3;
	CreateDisfellCommand();
	clickComp = CreateComponent<ClickComponent>();
}

BossRazer::~BossRazer()
{
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);
}

void BossRazer::BeginPlay()
{
	Super::BeginPlay();
	player = GetWorld()->FindActorByType<Player>();

}

void BossRazer::FixedUpdate(float _fixedRate)
{
	Super::FixedUpdate(_fixedRate);

	if (destroyThis)
	{
		Destroy();
		return;
	}
}

void BossRazer::Update(float _dt)
{
	Super::Update(_dt);

	dispelTime -= _dt;
	if (dispelTime <= 0.f)
	{
		ShutdownDispelChannel();
	}

	if (startShuttingDown)
	{
		shuttingDownTimer -= _dt;
		if (shuttingDownTimer <= 0.f)
		{
			lazerShuttingDown->Trigger(false);
			lazerShuttingDown->isVisible = false;
			DestroyThis();
		}

		return;
	}
	
	if (castTime >= 0.f)
	{
		castTime -= _dt;

		if (0.f <= castTime && castTime <= 2.f)
		{
			// TODO: Start charging lazer.
			if (!startCharging)
			{
				lazerCharging->isVisible = true;
				lazerCharging->Trigger(true);

				startCharging = true;
			}
		}

		return;
	}

	if (!obb->bGenerateOverlapEvent)
	{
		lazerCharging->Trigger(false);
		lazerCharging->isVisible = false;

		lazer->Trigger(true);
		lazer->isVisible = true;
		obb->bGenerateOverlapEvent = true;
	}

	// 스킬이 발동되고 있는 동안
	if (skillDuration >= 0.f)
	{
		skillDuration -= _dt;
		obb->bShouldOverlapTest = true;
		for (auto& [actor, f] : tickDamageTimerMap)
		{
			// 틱 데미지 업데이트
			f.Update(_dt);
		}
	}
	// 스킬 사망
	if (skillDuration < 0.f)
	{
		lazer->Trigger(false);
		lazer->isVisible = false;

		lazerShuttingDown->Trigger(true);
		lazerShuttingDown->isVisible = true;

		obb->SetCollisionEnabled(CollisionEnabled::NoCollision);
		startShuttingDown = true;
	}
}

bool BossRazer::Destroy()
{
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);

	return __super::Destroy();
}

void BossRazer::DisfellAction()
{
	DestroyThis();
}

void BossRazer::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void BossRazer::OnClicked()
{
	__super::OnClicked();

	if (dispelTime <= 0.f) return;

	LOG_MESSAGE("Razer Clicked!");
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void BossRazer::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		// 틱 데미지
		auto f = [this, other]() {
			other->TakeDamage(
				damage,
				razerDamageEvent,
				nullptr,
				this
			);
		};
		f();
		razerDamageEvent.origin = GetLocation();
		tickDamageTimerMap.insert({ other, TakeDamageTimer(f, tickInterval, true) });
	}
}

void BossRazer::OnEndOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		// 틱 데미지 끄기
		auto it = tickDamageTimerMap.find(other);
		if (it != tickDamageTimerMap.end())
		{
			it->second.SetFinish(true);
			tickDamageTimerMap.erase(it);
		}
	}
}


void BossRazer::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern07_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void BossRazer::ReflectionOut() {}

void BossRazer::DestroyThis()
{
	ShutdownDispelChannel();
	if (rootComponent->parent)
	{
		rootComponent->parent->RemoveChild(rootComponent);
		rootComponent->parent = nullptr;
	}
	destroyThis = true;
}

void BossRazer::ShutdownDispelChannel()
{
	if (!bShutdownDispelChannel)
	{
		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();
		bShutdownDispelChannel = true;
	}
}
