#include "Pattern06.h" 
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"

#include "D2DGameEngine/World.h"
#include "Player.h"

Pattern06::Pattern06(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();
	disfellCommandCount = 4;
	CreateDisfellCommand();
	CreateComponent<ClickComponent>();

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
	circleComponent->bSimulatePhysics = false;				// �����ӿ� ������ �����մϴ�.
	circleComponent->bApplyImpulseOnDamage = true;	// �������� ���� �� ����� ���մϴ�.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	circleComponent;	// ���� ������Ʈ�� ��Ʈ ������Ʈ�� �浹ü �Դϴ�.
	bm->AddChild(circleComponent);

	Pattern06DissfellEvent.SetBossSkillActor(this);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	Pattern06DamageEvent.SetDamageType(radiaDamageType);
	Pattern06DamageEvent.origin = GetLocation();
	Pattern06DamageEvent.radialDamageInfo.maxDamage = damage;
	Pattern06DamageEvent.radialDamageInfo.minDamage = damage;
	Pattern06DamageEvent.componentHits.resize(1);
}

void Pattern06::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/Pattern06_MagicCircle.png");
	bm->Translate(0, 200);
	circleComponent->InitCircleRadius(bm->GetSpriteHeight()/2);	// �������� 62�̰� ���̰� 110 �� ĸ�� �浹ü�� �ʱ�ȭ �մϴ�.
	circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void Pattern06::Update(float _dt)
{
	__super::Update(_dt);
	if (skillDuration > 0.f)
	{
		bm->Scale(1.0f + _dt * 0.1f, 1.0f + _dt * 0.1f);
		skillDuration -= _dt;
	}
	if (skillDuration < 0.f)
	{
		Pattern06DamageEvent.radialDamageInfo.innerRadius = 500.f;
		Pattern06DamageEvent.radialDamageInfo.outerRadius = 500.f;
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
		Pattern06DamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;
		player->TakeDamage(damage, Pattern06DamageEvent, nullptr, this);
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}

void Pattern06::DisfellAction()
{
	bm->Scale(0.9f, 0.9f);
}

void Pattern06::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern06_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void Pattern06::ReflectionOut() {}
