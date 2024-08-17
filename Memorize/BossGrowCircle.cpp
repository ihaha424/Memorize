#include "BossGrowCircle.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"
#include "TestLevel1_RenderLayer.h"
#include "D2DGameEngine/World.h"
#include "Player.h"

BossGrowCircle::BossGrowCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent->AddChild(abm);
	bm->isVisible = false;

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
	circleComponent->bSimulatePhysics = false;				// �����ӿ� ������ �����մϴ�.
	circleComponent->bApplyImpulseOnDamage = true;	// �������� ���� �� ����� ���մϴ�.
	circleComponent->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	circleComponent;	// ���� ������Ʈ�� ��Ʈ ������Ʈ�� �浹ü �Դϴ�.
	bm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowCircleDamageEvent.origin = GetLocation();
	BossGrowCircleDamageEvent.damageEventType = DamageEventType::RadialDamage;
	BossGrowCircleDamageEvent.radialDamageInfo.maxDamage = 0.f;
	BossGrowCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = 749.f;
	BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = 750.f;
	BossGrowCircleDamageEvent.componentHits.resize(1);

	scaleTween = new DotTween<float>(&scaleVarias, EasingEffect::Linear, StepAnimation::StepOnceForward);
	scaleTween->SetDuration(2.f);
	scaleTween->SetStartPoint(1.f);
	scaleTween->SetEndPoint(0.f);
}

void BossGrowCircle::BeginPlay()
{
	__super::BeginPlay();
	
	{
		abm->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowCircle.png");
		abm->SliceSpriteSheet(1300, 1000, 0, 0, 0, 0);
		abm->SetFrameDurations({ 0.084f });
		abm->Trigger(true);
	}


	circleComponent->InitCircleRadius(750.f);
	//circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossGrowCircle::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
}

void BossGrowCircle::Update(float _dt)
{
	__super::Update(_dt);

	if (startSkill > 0.f)
	{
		startSkill -= _dt;
		return;
	}
	skillDuration -= _dt;
	scaleTween->Update(_dt);
	//bm->SetScale(scaleVarias, scaleVarias);

	circleComponent->InitCircleRadius(750.f * scaleVarias);
	circleComponent->bShouldOverlapTest = !circleComponent->bShouldOverlapTest;
	for (auto& [actor, f] : tickDamageTimerMap)
	{
		// ƽ ������ ������Ʈ
		f.Update(_dt);
	}
	if (skillDuration < 0.f)
	{
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}

void BossGrowCircle::OnBeginOverlap(Actor* other)
{
	if (other)
	{
		auto f = [this, other]() {
			BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = 749.f * scaleVarias;
			BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = 750.f * scaleVarias;
			other->TakeDamage(
				damage,
				BossGrowCircleDamageEvent,
				nullptr,
				this
			);
		};
		f();
		tickDamageTimerMap.insert({ other, TakeDamageTimer(f, tickInterval, true) });
	}
}

void BossGrowCircle::OnEndOverlap(Actor* other)
{
	if (other)
	{
		// ƽ ������ ����
		auto it = tickDamageTimerMap.find(other);
		if (it != tickDamageTimerMap.end())
		{
			it->second.SetFinish(true);
			tickDamageTimerMap.erase(it);
		}
	}
}


void BossGrowCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern02_Actor.txt");
	reflectionResource->ParsingFile(0, skillDuration, damage);
}

void BossGrowCircle::ReflectionOut()
{
}
