#include "BossProjectile.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "MovementComponent.h"
#include "D2DGameEngine/World.h"
#include "Player.h"

BossProjectile::BossProjectile(World* _world, std::wstring fileName)
	:BossSkillActor(_world)
{
	reflectionResource = ResourceManager::LoadResource<ReflectionResource>(fileName);


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
	BossProjectileDamageEvent.SetDamageType(radiaDamageType);
	BossProjectileDamageEvent.origin = GetLocation();
	BossProjectileDamageEvent.radialDamageInfo.maxDamage = damage;
	BossProjectileDamageEvent.radialDamageInfo.minDamage = damage;
	BossProjectileDamageEvent.componentHits.resize(1);
}

void BossProjectile::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/BossProjectile.png");
	bm->Translate(0, 200);
	circleComponent->InitCircleRadius(bm->GetSpriteHeight() / 2);	// �������� 62�̰� ���̰� 110 �� ĸ�� �浹ü�� �ʱ�ȭ �մϴ�.
	circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossProjectile::Update(float _dt)
{
	__super::Update(_dt);
	if (skillDuration < 0.f)
	{
		BossProjectileDamageEvent.radialDamageInfo.innerRadius = 500.f;
		BossProjectileDamageEvent.radialDamageInfo.outerRadius = 500.f;
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
		BossProjectileDamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;
		player->TakeDamage(damage, BossProjectileDamageEvent, nullptr, this);
		SetStatus(EObjectStatus::OS_DESTROY);
	}
	//if (���� ����Ʈ -> ����Ʈ�� �� ����? ���ӽð�)
	//{
	//	SetStatus(EObjectStatus::OS_DESTROY);
	//}
}

void BossProjectile::ReflectionIn()
{
	reflectionResource->ParsingFile(0, 
		isFragile,
		dispelTime, 
		skillDuration, 
		damage,
		speed,
		duration);
}

void BossProjectile::ReflectionOut()
{
}

void BossProjectile::SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _direction)
{
	SetLocation(_startPos.x, _startPos.y);
	mv->SetDirection(_direction);
}
