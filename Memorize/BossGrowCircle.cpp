#include "BossGrowCircle.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "Player.h"

BossGrowCircle::BossGrowCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();
	disfellCommandCount = 2;
	CreateDisfellCommand();

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
	circleComponent->bSimulatePhysics = false;				// �����ӿ� ������ �����մϴ�.
	circleComponent->bApplyImpulseOnDamage = true;	// �������� ���� �� ����� ���մϴ�.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	circleComponent;	// ���� ������Ʈ�� ��Ʈ ������Ʈ�� �浹ü �Դϴ�.
	bm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowCircleDamageEvent.origin = GetLocation();
	BossGrowCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = 750.f;
	BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = 750.f;
	BossGrowCircleDamageEvent.componentHits.resize(1);

	for (int i = 0; i < disfellCommand.size(); i++)
		std::cout << (int)disfellCommand[i] + 1 << ", ";

	scaleTween = new DotTween<float>(&scaleVarias, EasingEffect::Linear, StepAnimation::StepOnceForward);
	scaleTween->SetDuration(2.f);
	scaleTween->SetStartPoint(1.f);
	scaleTween->SetEndPoint(0.f);
}

void BossGrowCircle::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/Pattern06_MagicCircle.png");
	bm->Translate(0, 200);
	circleComponent->InitCircleRadius(bm->GetSpriteHeight() / 2);	// �������� 62�̰� ���̰� 110 �� ĸ�� �浹ü�� �ʱ�ȭ �մϴ�.
	//circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
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
	bm->SetScale(750.f * scaleVarias, 750.f * scaleVarias);
	if (skillDuration < 0.f)
	{
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}

void BossGrowCircle::OnBeginOverlap(Actor* other)
{
	other->TakeDamage(damage, BossGrowCircleDamageEvent, nullptr, this);
}

void BossGrowCircle::ReflectionIn()
{
}

void BossGrowCircle::ReflectionOut()
{
}
