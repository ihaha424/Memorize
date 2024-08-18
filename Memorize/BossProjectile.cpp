#include "BossProjectile.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "MovementComponent.h"
#include "D2DGameEngine/World.h"
#include "Player.h"

BossProjectile::BossProjectile(World* _world)
	:BossSkillActor(_world)
{}

void BossProjectile::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/BossProjectile.png");
	circleComponent->InitCircleRadius(bm->GetSpriteHeight() / 2);	// 반지름이 62이고 높이가 110 인 캡슐 충돌체를 초기화 합니다.
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
	//if (폭팔 이펙트 -> 이펙트로 할 수도? 지속시간)
	//{
	//	SetStatus(EObjectStatus::OS_DESTROY);
	//}
}

void BossProjectile::SetActor(std::wstring _fileName)
{
	reflectionResource = ResourceManager::LoadResource<ReflectionResource>(_fileName);


	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyProjectile);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap 이벤트를 발생시킵니다.
	circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
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
