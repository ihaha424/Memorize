#include "BossRazer.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"

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

	magicCircleHub = CreateComponent<SceneComponent>();
	rootComponent = magicCircleHub;

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
	razer = CreateComponent<AnimationBitmapComponent>();
	razer->SetSprite(L"TestResource/Boss/BossRazer/Razer.png");
	razer->SetLoop(true);
	razer->SliceSpriteSheet(12, 6, 0, 0, 4, 0);
	razer->SetFrameDurations({ 0.2 });
	razer->Pause();	// Resume
	razer->isVisible = false;	// True
	rootComponent->AddChild(razer);
	obb = CreateComponent<PolygonComponent>();
	obb->InitPolygon({ {-6, -3}, {6, -3}, {6, 3}, {-6, 3} });
	razer->AddChild(obb);
	obb->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);
	obb->bApplyImpulseOnDamage = true;
	obb->bGenerateOverlapEvent = false;	// True
	razer->Translate(-1200.f, 0.f);
	razer->Scale(200.f, 30.f);

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

	if (castTime >= 0.f)
	{
		castTime -= _dt;
		return;
	}

	if (!obb->bGenerateOverlapEvent)
	{
		razer->Resume();
		razer->isVisible = true;
		obb->bGenerateOverlapEvent = true;
	}

	// 스킬이 발동되고 있는 동안
	if (skillDuration >= 0.f)
	{
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
		DestroyThis();
	}
}

void BossRazer::DisfellAction()
{
	DestroyThis();
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
	GetWorld()->UnregisterComponentCollision(obb);
	destroyThis = true;
}
