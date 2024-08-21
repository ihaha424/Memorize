#include "RazerCircle.h"

#include "D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Level.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/CircleComponent.h"
#include "D2DGameEngine/ClickComponent.h"

#include "BossRazer.h"

RazerCircle::RazerCircle(World* _world) : BossSkillActor(_world)
{
	renderLayer = 1;

	bm->isVisible = false;

	isDispel = true;
	isFragile = false;
	type = Range;
	dispelTime = 13.f;
	skillDuration = 15.f;
	castingTime = 0.f;
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
	circle->InitCircleRadius(520);
	circle->SetScale(0.8f, 0.8f);


	attach1 = CreateComponent<SceneComponent>();
	rootComponent->AddChild(attach1);
	attach1->SetRotation(0.f);
	razer1 = GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
	attach1->AddChild(razer1->rootComponent);
	razer1->skillDuration = 10.f;
	razer1->castTime = 2.f;
	razer1->magicCircle->SetScale(.2f, .2f);
	razer1->SetScale(2.f, 2.f);
	razer1->SetRotation(-90.f);
	razer1->SetLocation(0.f, 500.f);
	razer1->renderLayer = 5;
	razer1->clickComp->ownerBounds = &bm->bounds;

	attach2 = CreateComponent<SceneComponent>();
	rootComponent->AddChild(attach2);
	attach2->SetRotation(120.f);
	razer2 = GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
	attach2->AddChild(razer2->rootComponent);
	razer2->skillDuration = 10.f;
	razer2->castTime = 2.f;
	razer2->magicCircle->SetScale(.2f, .2f);
	razer2->SetScale(2.f, 2.f);
	razer2->SetRotation(-90.f);
	razer2->SetLocation(0.f, 500.f);
	razer2->renderLayer = 5;
	razer2->clickComp->ownerBounds = &bm->bounds;

	attach3 = CreateComponent<SceneComponent>();
	rootComponent->AddChild(attach3);
	attach3->SetRotation(240.f);
	razer3 = GetWorld()->GetCurLevel()->CreateActor<BossRazer>();
	attach3->AddChild(razer3->rootComponent);
	razer3->skillDuration = 10.f;
	razer3->castTime = 2.f;
	razer3->magicCircle->SetScale(.2f, .2f);
	razer3->SetScale(2.f, 2.f);
	razer3->SetRotation(-90.f);
	razer3->SetLocation(0.f, 500.f);
	razer3->renderLayer = 5;
	razer3->clickComp->ownerBounds = &bm->bounds;

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

	disfellCommandCount = 4;
	CreateDisfellCommand();
	CreateComponent<ClickComponent>();
}

void RazerCircle::BeginPlay()
{
	Super::BeginPlay();

	// Place circle
	circle->SetTranslation(GET_MAP_CENTER().x, GET_MAP_CENTER().y);
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

	dispelTime -= _dt;
	if (dispelTime <= 0.f)
	{
		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();
	}
}

void RazerCircle::DisfellAction()
{
	DestroyThis();
}

void RazerCircle::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void RazerCircle::OnClicked()
{
	__super::OnClicked();

	if (dispelTime <= 0.f) return;

	LOG_MESSAGE("Circle Clicked!");
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
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
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern10_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void RazerCircle::ReflectionOut() {}

void RazerCircle::DestroyThis()
{
	//if (razer1->GetStatus() != EObjectStatus::OS_DESTROY)
	//	razer1->DestroyThis();
	//if (razer2->GetStatus() != EObjectStatus::OS_DESTROY)
	//	razer2->DestroyThis();
	//if (razer3->GetStatus() != EObjectStatus::OS_DESTROY)
	//	razer3->DestroyThis();

	//attach1->RemoveChild(razer1->rootComponent);
	//attach2->RemoveChild(razer2->rootComponent);
	//attach3->RemoveChild(razer3->rootComponent);
	//GetWorld()->UnregisterComponentCollision(circle);
	//destroyThis = true;

	EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();

	razer3->Destroy();
	razer2->Destroy();
	razer1->Destroy();
	Destroy();
}
