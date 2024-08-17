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

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
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
	scaleTween->SetDuration(10.f);
	skillDuration = 10.f;
	scaleTween->SetStartPoint(1.f);
	scaleTween->SetEndPoint(0.f);
}

void BossGrowCircle::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/Pattern06_MagicCircle.png");
	circleComponent->InitCircleRadius(bm->GetSpriteHeight() / 2);	// �������� 62�̰� ���̰� 110 �� ĸ�� �浹ü�� �ʱ�ȭ �մϴ�.
	//circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossGrowCircle::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
	circleComponent->bShouldOverlapTest = true;
}

void BossGrowCircle::Update(float _dt)
{
	__super::Update(_dt);

	if (startSkill < 1.f)
	{
		startSkill += _dt;
		return;
	}
	skillDuration -= _dt;

	scaleTween->Update(_dt);
	bm->SetScale(scaleVarias, scaleVarias);
	if (skillDuration < 0.f)
	{
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}

void BossGrowCircle::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = 750.f * scaleVarias;
	BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = 750.f * scaleVarias;
	other->TakeDamage(damage, BossGrowCircleDamageEvent, nullptr, this);
}

void BossGrowCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern02_Actor.txt");
	reflectionResource->ParsingFile(0, skillDuration, damage);
}

void BossGrowCircle::ReflectionOut()
{
}
