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
	box->bSimulatePhysics = false;	// �����ӿ� ������ �������� �ʽ��ϴ�.
	box->bApplyImpulseOnDamage = false;	// �������� ���� �� ����� ���մϴ�.
	box->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
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
