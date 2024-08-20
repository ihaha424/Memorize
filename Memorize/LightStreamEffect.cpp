#include "LightStreamEffect.h"
#include "D2DGameEngine/PolygonComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/DamageEvent.h"

LightStreamEffect::LightStreamEffect(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);
	rootComponent = anim = CreateComponent<Animator>();
	initialState = anim->CreateState<AnimationState>();
	normalState = anim->CreateState<AnimationState>();
	endingState = anim->CreateState<AnimationState>();

	initialState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream01.png");
	initialState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	initialState->FrameResize(25);
	initialState->SetFrameDurations({ 2.f / 25 });

	normalState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream02.png");
	normalState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	normalState->FrameResize(60);
	normalState->SetFrameDurations({ 5.f / 60 });

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream03.png");
	endingState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	endingState->FrameResize(15);
	endingState->SetFrameDurations({ 1.f / 15 });

	anim->Initialize(initialState);

	obb = CreateComponent<PolygonComponent>();
	obb->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	obb->SetCollisionObjectType(ECollisionChannel::PlayerPattern);
	obb->bSimulatePhysics = false;	// 움직임에 물리를 적용하지 않습니다.
	obb->bApplyImpulseOnDamage = false;	// 데미지를 받을 때 충격을 가합니다.
	obb->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	obb->SetPolygon({ {-500.f, -80.f}, {500.f, -80.f }, {-500.f, 80.f}, {500.f, 80.f} });
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
		//대미지를 입힘
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

	obb->bShouldOverlapTest = true;
	damageTimer += _dt;
	if (state == State::Initial || state == State::Ending)
	{
		obb->bShouldOverlapTest = false;
	}

	if (elapsedTime > initialTime && state == State::Initial)
	{
		state = State::Normal;
		anim->SetState(normalState);	
	}
	else if (elapsedTime >= initialTime + duration && state == State::Normal)
	{
		state = State::Ending;
		anim->SetState(endingState);
	}
	if (elapsedTime >= initialTime + duration + endingTime)
	{
		Inactivate();
	}
}

LightStreamEffect::~LightStreamEffect()
{
}
