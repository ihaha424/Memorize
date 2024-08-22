#include "LightStreamEffect.h"
#include "D2DGameEngine/PolygonComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "Player.h"

LightStreamEffect::LightStreamEffect(World* _world) : SkillActor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);
	rootComponent = anim = CreateComponent<Animator>();
	initialState = anim->CreateState<AnimationState>();
	normalState = anim->CreateState<AnimationState>();
	endingState = anim->CreateState<AnimationState>();

	initialState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream01.png");
	initialState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	initialState->FrameResize(25);
	initialState->SetFrameDurations({ 0.5f / 25 });

	normalState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream02.png");
	normalState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	normalState->FrameResize(60);
	normalState->SetFrameDurations({ 5.f / 60 });
	normalState->SetLoop(true);

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream03.png");
	endingState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	endingState->FrameResize(15);
	endingState->SetFrameDurations({ 0.5f / 15 });

	anim->Initialize(initialState);

	obb = CreateComponent<PolygonComponent>();
	obb->collisionProperty = CollisionProperty(CollisionPropertyPreset::PlayerPattern);
	obb->SetCollisionObjectType(ECollisionChannel::PlayerPattern);
	obb->collisionProperty.SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Overlap);
	obb->collisionProperty.SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Ignore);
	obb->bSimulatePhysics = false;	// �����ӿ� ������ �������� �ʽ��ϴ�.
	obb->bApplyImpulseOnDamage = false;	// �������� ���� �� ����� ���մϴ�.
	obb->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	obb->SetPolygon({ {-500.f, -80.f}, {500.f, -80.f }, {500.f, 80.f}, {-500.f, 80.f} });
	rootComponent->AddChild(obb);
}

void LightStreamEffect::Initialize()
{
	state = State::Initial;
	anim->SetState(initialState);
	elapsedTime = 0.f;
	damageTimer = 0.f;
}

void LightStreamEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void LightStreamEffect::OnOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnOverlap(other, overlap);

	if (damageTimer > damageInterval)
	{
		damageTimer = 0.f;
		//������� ����
		DamageEvent damageEvent;
		DamageType damageType
		{
			.damageImpulse = 10000
		};
		damageEvent.SetDamageType(damageType);

		other->TakeDamage(damage, damageEvent, nullptr, this);
	}

}

void LightStreamEffect::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	
	damageTimer += _dt;

	if (elapsedTime > initialTime && state == State::Initial)
	{
		state = State::Normal;
		anim->SetState(normalState);	
		GetWorld()->FindActorByType<Player>()->orb->SetStatus(OS_ACTIVE);
	}
	else if (elapsedTime >= initialTime + duration && state == State::Normal)
	{
		state = State::Ending;
		anim->SetState(endingState);
		
	}
	else
	{
		obb->bShouldOverlapTest = true;
	}
	if (elapsedTime >= initialTime + duration + endingTime)
	{
		Inactivate();
	}
}

LightStreamEffect::~LightStreamEffect()
{
}
