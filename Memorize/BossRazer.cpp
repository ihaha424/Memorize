#include "BossRazer.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"

#include "Player.h"

BossRazer::BossRazer(World* _world) : BossSkillActor(_world)
{
	bm->isVisible = false;

	isDispel = true;
	type = Range;
	dispelTime = 7.f;
	skillDuration = 1000.f;
	// 스킬 시전 타이밍은 std::max(DispelTime - SkillDuration, 0);
	damage = 17.f;

	magicCircleHub = CreateComponent<SceneComponent>();
	rootComponent = magicCircleHub;

	magicCircle1 = CreateComponent<BitmapComponent>();
	magicCircle1->SetSprite(L"TestResource/Boss/BossRazer/MagicCircle1.png");
	magicCircleHub->AddChild(magicCircle1);
	magicCircle2 = CreateComponent<BitmapComponent>();
	magicCircle2->SetSprite(L"TestResource/Boss/BossRazer/MagicCircle2.png");
	magicCircleHub->AddChild(magicCircle2);
	magicCircle3 = CreateComponent<BitmapComponent>();
	magicCircle3->SetSprite(L"TestResource/Boss/BossRazer/MagicCircle3.png");
	magicCircleHub->AddChild(magicCircle3);
	magicCircle4 = CreateComponent<BitmapComponent>();
	magicCircle4->SetSprite(L"TestResource/Boss/BossRazer/MagicCircle4.png");
	magicCircleHub->AddChild(magicCircle4);

	razer = CreateComponent<AnimationBitmapComponent>();
	razer->SetSprite(L"TestResource/Boss/BossRazer/Razer.png");
	razer->SetLoop(true);
	razer->SliceSpriteSheet(12, 6, 0, 0, 4, 0);
	razer->SetFrameDurations({ 0.2 });
	rootComponent->AddChild(razer);
	obb = CreateComponent<PolygonComponent>();
	obb->InitPolygon({ {-6, -3}, {6, -3}, {6, 3}, {-6, 3} });
	razer->AddChild(obb);
	obb->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	obb->bGenerateOverlapEvent = true;

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

}

void BossRazer::Update(float _dt)
{
	Super::Update(_dt);

	// 스킬이 발동되고 있는 동안
	if (skillDuration >= 0.f)
	{
		obb->bShouldOverlapTest = true;
		for (auto [actor, f] : tickDamageTimerMap)
		{
			// 틱 데미지 업데이트
			f.Update(_dt);
		}
	}
	// 스킬 사망
	if (skillDuration < 0.f)
	{
		Destroy();
	}
}

void BossRazer::DisfellAction()
{
	Destroy();
}

void BossRazer::OnBeginOverlap(Actor* other)
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

void BossRazer::OnEndOverlap(Actor* other)
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