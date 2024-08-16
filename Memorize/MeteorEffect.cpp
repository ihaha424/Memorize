#include "MeteorEffect.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "MovementComponent.h"

MeteorEffect::MeteorEffect(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);
}

MeteorEffect::~MeteorEffect()
{
}

void MeteorEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void MeteorEffect::Update(float _dt)
{
	__super::Update(_dt);

	if (Math::Vector2::Distance(GetLocation(), attackPos) < 30)
	{
		Inactivate();

		//TODO
		//대미지 입히기 
	}
}

void MeteorEffect::Initialize()
{
}
