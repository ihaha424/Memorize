#include "MeteorEffect.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "MovementComponent.h"
#include "D2DGameEngine/PolygonComponent.h"
#include "D2DGameEngine/DamageEvent.h"

MeteorEffect::MeteorEffect(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_Meteor01.png");
	normalState->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
	normalState->SetFrameDurations({ 3.f/96 });
	anim->Initialize(normalState);

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_Meteor03.png");
	endingState->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
	endingState->SetFrameDurations({ 2.f/48 });

	//box collider 재설정
	box->SetCollisionObjectType(ECollisionChannel::PlayerPattern);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Overlap);
	box->SetStatus(OS_INACTIVE);
	box->SetPolygon({ {-80, -150}, {80, -150}, {80, 150}, {-80, 150} });

	duration = 10.f;
	bIsPassable = true;
	endingTime = 2.f;
	bIsPassable = true;
	bCollideWithOtherAttack = false;
}

MeteorEffect::~MeteorEffect()
{
}

void MeteorEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void MeteorEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);
}

void MeteorEffect::Update(float _dt)
{
	__super::Update(_dt);

	box->bShouldOverlapTest = true;

	if (Math::Vector2::Distance(GetLocation(), attackPos) < 20)
	{
		if (bEnding) return;
		//위치에 도달하면 폭발하고 collider를 켠다.
		mv->SetSpeed(0);
		anim->SetState(endingState);
		box->SetStatus(OS_ACTIVE);
		bEnding = true;
		elapsedTime = duration + delay;
	}
}

void MeteorEffect::Initialize()
{
	__super::Initialize();

	anim->SetState(normalState);
}
