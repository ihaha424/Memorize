#include "ElementalExplosionEffect.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/DamageEvent.h"

ElementalExplosionEffect::ElementalExplosionEffect(World* _world) : SkillActor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	rootComponent = bm = CreateComponent<BitmapComponent>();
	bm->SetSprite(L"TestResource/Skill/Range/TighteningCircle.png");

	box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	box->bSimulatePhysics = false;	// 움직임에 물리를 적용하지 않습니다.
	box->bApplyImpulseOnDamage = false;	// 데미지를 받을 때 충격을 가합니다.
	box->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	box->SetCollisionObjectType(ECollisionChannel::PlayerProjectile);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Block);
	box->SetBoxExtent({3000.f, 3000.f});
	rootComponent->AddChild(box);
}
void ElementalExplosionEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void ElementalExplosionEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	other->Destroy();
}

void ElementalExplosionEffect::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;
	if (elapsedTime > duration)
	{
		elapsedTime = 0.f;
		Inactivate();
	}
}

ElementalExplosionEffect::~ElementalExplosionEffect()
{
}
