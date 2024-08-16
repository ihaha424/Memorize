#include "ChasingWaterBallProjectile.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/Character.h"

ChasingWaterBallProjectile::ChasingWaterBallProjectile(World* _world)
	: Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);

	Inactivate();
}

ChasingWaterBallProjectile::~ChasingWaterBallProjectile()
{

}

void ChasingWaterBallProjectile::Update(float _dt)
{
	__super::Update(_dt);

	if (state == State::Idle)
	{
		xValue.Update(_dt);
		yValue.Update(_dt);
		SetLocation(x, y);
	}
	else if (state == State::Chase)
	{

	}
	else
	{

	}
}

void ChasingWaterBallProjectile::Initialize()
{
	xValue.SetData(&x);
	xValue.SetDuration(1.f);
	xValue.SetStartPoint(GetLocation().x + 100);
	xValue.SetEndPoint(GetLocation().x - 100);
	xValue.SetEasingEffect(EasingEffect::Linear);
	xValue.SetStepAnimation(StepAnimation::StepLoopPingPong);

	yValue.SetData(&y);
	yValue.SetDuration(0.5f);
	yValue.SetStartPoint(GetLocation().y - 5);
	yValue.SetEndPoint(GetLocation().y + 5);
	yValue.SetEasingEffect(EasingEffect::InOutBounce);
	yValue.SetStepAnimation(StepAnimation::StepLoopPingPong);

}
