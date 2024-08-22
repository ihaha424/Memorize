#include "BossThroughProjectile.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "../D2DGameEngine/AnimationEffect.h"
#include "Player.h"

BossThroughProjectile::BossThroughProjectile(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	bm->isVisible = false;
	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	abm->MarkBoundsDirty();
	mv = CreateComponent<MovementComponent>();
	abm->AddChild(mv);

	circleComponent = CreateComponent<CircleComponent>();
	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyProjectile);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
	circleComponent->bSimulatePhysics = false;				// �����ӿ� ������ �����մϴ�.
	circleComponent->bApplyImpulseOnDamage = true;	// �������� ���� �� ����� ���մϴ�.
	circleComponent->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	circleComponent->SetCollisionObjectType(ECollisionChannel::EnemyProjectile);
	circleComponent->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	circleComponent->collisionProperty.SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Block);
	circleComponent->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Block);
	circleComponent->bShouldOverlapTest = true;

	abm->AddChild(circleComponent);

	DamageType radiaDamageType{
	.damageImpulse = 10000.f,
	};
	BossThroughProjectileDamageEvent.SetDamageType(radiaDamageType);
	BossThroughProjectileDamageEvent.origin = GetLocation();
	BossThroughProjectileDamageEvent.radialDamageInfo.maxDamage = damage;
	BossThroughProjectileDamageEvent.radialDamageInfo.minDamage = damage;
	BossThroughProjectileDamageEvent.componentHits.resize(1);

	speedTween = new DotTween<float>(&speedVarias, EasingEffect::InExpo, StepAnimation::StepOnceForward);
	speedTween->SetDuration(0.5f);
	speedTween->SetStartPoint(0.f);
	speedTween->SetEndPoint(speed);
}

void BossThroughProjectile::BeginPlay()
{
	__super::BeginPlay();
	
	{
		abm->SetSprite(L"TestResource/Boss/ThroughProjectileEffect/Boss_ThroughProjectile.png");
		abm->SliceSpriteSheet(240, 181, 0, 0, 0, 0);
		abm->FrameResize(120);
		abm->SetFrameDurations({ 0.016f });
		abm->SetLoop(true);
		abm->Trigger(true);
	}

	circleComponent->InitCircleRadius(150 / 2);	// �������� 62�̰� ���̰� 110 �� ĸ�� �浹ü�� �ʱ�ȭ �մϴ�.

	player = GetWorld()->FindActorByType<Player>();

}

void BossThroughProjectile::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);

	// �ݸ��� �̺�Ʈ �ѷ����� ��� ��ũ.
	if (destroyThis)
	{
		Destroy();
		return;
	}
}

void BossThroughProjectile::Update(float _dt)
{
	__super::Update(_dt);

	if (destroyThis) return;

	skillDuration -= _dt;
	speedTween->Update(_dt);
	mv->SetSpeed(speedVarias);

	if (skillDuration < 0.f)
	{
		// ���� FixedUpdate ƽ �������� ������Ʈ �����ϰ� ����.
		
		// �ݸ��� ������
		circleComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);
		circleComponent->bGenerateOverlapEvent = false;
		Destroy();
	}
}

bool BossThroughProjectile::Destroy()
{
	//BlickSource Effect
	{
		AnimationEffect* DestoryProjectileEffect = GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		DestoryProjectileEffect->SetSprite(L"TestResource/Boss/ThroughProjectileEffect/Boss_ThroughProjectileEffect.png");
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(300, 300, 0, 0, 0, 0);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 0.025f });
		DestoryProjectileEffect->GetAnimationBitmapComponent()->Trigger(true);
		DestoryProjectileEffect->SetAliveTime(1.f);
		auto Pos = GetLocation();
		DestoryProjectileEffect->SetLocation(Pos.x, Pos.y);
	}


	return __super::Destroy();
}

void BossThroughProjectile::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	other->TakeDamage(
		damage,
		BossThroughProjectileDamageEvent,
		nullptr,
		this
	);
	circleComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);
	circleComponent->bGenerateOverlapEvent = false;

	Destroy();
}


void BossThroughProjectile::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern09_Actor.txt");
	reflectionResource->ParsingFile(0, dispelTime, skillDuration, damage, speed, duration);
}

void BossThroughProjectile::ReflectionOut()
{}

void BossThroughProjectile::SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _direction)
{
	SetLocation(_startPos.x, _startPos.y);
	mv->SetDirection(_direction);
	
	Math::Vector2 w = _direction;
	Math::Vector2 v = -Right();

	float x = w.y * v.x - w.x * v.y;
	float y = w.x * v.x + w.y * v.y;
	float rad = atan2(x, y);

	Rotate(Math::RadianToDegree(rad));
}
