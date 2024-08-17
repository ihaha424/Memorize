#include "RazerCircle.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Level.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/CircleComponent.h"

#include "BossRazer.h"

RazerCircle::RazerCircle(World* _world) : BossSkillActor(_world)
{
	bm->isVisible = false;

	isDispel = true;
	isFragile = false;
	type = Range;
	dispelTime = 0.f;
	skillDuration = 10.f;
	damage = 5.f;
	speed = 0.f;
	duration = 0.f;

	circle = CreateComponent<CircleComponent>();
	circle->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);
	circle->bApplyImpulseOnDamage = true;
	circle->bGenerateOverlapEvent = false;	// True
	rootComponent = circle;

	AnimationBitmapComponent* centerCircle = CreateComponent<AnimationBitmapComponent>();
	circle->AddChild(centerCircle);
	centerCircle->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowMagicCircle.png");
	centerCircle->SliceSpriteSheet(1200, 1200, 0, 0, 0, 0);
	centerCircle->SetFrameDurations({ 1.f / 12.f });
	centerCircle->FrameResize(82);
	centerCircle->Trigger(true);
	centerCircle->SetLoop(true);
	circle->InitCircleRadius(800);
	circle->SetScale(0.8f, 0.8f);


	SceneComponent* attach1 = CreateComponent<SceneComponent>();
	rootComponent->AddChild(attach1);
	attach1->SetRotation(0.f);
	razer1 = GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
	attach1->AddChild(razer1->rootComponent);
	razer1->magicCircle->SetScale(.2f, .2f);
	razer1->SetScale(2.f, 2.f);
	razer1->SetRotation(-90.f);
	razer1->SetLocation(0.f, 500.f);
	razer1->renderLayer = 5;

	SceneComponent* attach2 = CreateComponent<SceneComponent>();
	rootComponent->AddChild(attach2);
	attach2->SetRotation(120.f);
	razer2 = GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
	attach2->AddChild(razer2->rootComponent);
	razer2->magicCircle->SetScale(.2f, .2f);
	razer2->SetScale(2.f, 2.f);
	razer2->SetRotation(-90.f);
	razer2->SetLocation(0.f, 500.f);
	razer2->renderLayer = 5;

	SceneComponent* attach3 = CreateComponent<SceneComponent>();
	rootComponent->AddChild(attach3);
	attach3->SetRotation(240.f);
	razer3 = GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
	attach3->AddChild(razer3->rootComponent);
	razer3->magicCircle->SetScale(.2f, .2f);
	razer3->SetScale(2.f, 2.f);
	razer3->SetRotation(-90.f);
	razer3->SetLocation(0.f, 500.f);
	razer3->renderLayer = 5;

	// Damage Event
	DamageType damageType{
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
		1000.f
	);
	circleDamage.radialDamageInfo = damageInfo;
}

void RazerCircle::BeginPlay()
{
	Super::BeginPlay();

	// Rotate circle
	SetAngularVelocity(angularVelocity);
}

void RazerCircle::FixedUpdate(float _fixedRate)
{
	Super::FixedUpdate(_fixedRate);

	if (destroyThis)
	{
		Destroy();
		return;
	}
}

void RazerCircle::Update(float _dt)
{
	Super::Update(_dt);

	if (destroyThis) return;

	castingTime -= _dt;
	if (castingTime <= 0.f)
	{
		castingTime = -1.f;

		skillDuration -= _dt;
		if (skillDuration <= 0.f)
		{
			DestroyThis();
		}
		else
		{
			circle->bGenerateOverlapEvent = true;
			circle->bShouldOverlapTest = true;
			for (auto& [actor, f] : tickDamageTimerMap)
			{
				// Æ½ µ¥¹ÌÁö ¾÷µ¥ÀÌÆ®
				f.Update(_dt);
			}
		}
	}
}

void RazerCircle::DisfellAction()
{
	DestroyThis();
}

void RazerCircle::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		// Æ½ µ¥¹ÌÁö
		auto f = [this, other]() {
			other->TakeDamage(
				damage,
				circleDamage,
				nullptr,
				this
			);
		};
		f();
		circleDamage.origin = GetLocation();
		tickDamageTimerMap.insert({ other, TakeDamageTimer(f, tickInterval, true) });
	}
}

void RazerCircle::OnEndOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		// Æ½ µ¥¹ÌÁö ²ô±â
		auto it = tickDamageTimerMap.find(other);
		if (it != tickDamageTimerMap.end())
		{
			it->second.SetFinish(true);
			tickDamageTimerMap.erase(it);
		}
	}
}

void RazerCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern07_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void RazerCircle::ReflectionOut() {}

void RazerCircle::DestroyThis()
{
	razer1->DestroyThis();
	razer2->DestroyThis();
	razer3->DestroyThis();

	GetWorld()->UnregisterComponentCollision(circle);
	destroyThis = true;
}
