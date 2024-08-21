#include "ElementalExplosionEffect.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/DamageEvent.h"


#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/AnimationEffect.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"


ElementalExplosionEffect::ElementalExplosionEffect(World* _world) : SkillActor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	rootComponent = box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	box->bSimulatePhysics = false;	// �����ӿ� ������ �������� �ʽ��ϴ�.
	box->bApplyImpulseOnDamage = false;	// �������� ���� �� ����� ���մϴ�.
	box->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	box->SetCollisionObjectType(ECollisionChannel::PlayerProjectile);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Block);
	box->SetBoxExtent({3000.f, 3000.f});
}
void ElementalExplosionEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void ElementalExplosionEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	other->Destroy();
	Math::Vector2 DestoryProjectileEPos = other->GetLocation();
	//BlickSource Effect
	{
		AnimationEffect* DestoryProjectileEffect = GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		DestoryProjectileEffect->SetSprite(L"TestResource/Player/Skill/Skill_ElementalExplosion1.png");
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(300, 300, 0, 0, 0, 0);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 0.025f });
		DestoryProjectileEffect->GetAnimationBitmapComponent()->Trigger(true);
		DestoryProjectileEffect->SetAliveTime(1.f);
		DestoryProjectileEffect->SetLocation(DestoryProjectileEPos.x, DestoryProjectileEPos.y);
	}
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
