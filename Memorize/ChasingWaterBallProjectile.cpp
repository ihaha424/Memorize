#include "ChasingWaterBallProjectile.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/Character.h"

ChasingWaterBallProjectile::ChasingWaterBallProjectile(World* _world)
	: Projectile(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);

	BitmapComponent* bm = GetComponent<BitmapComponent>();
	bm->SetSprite(L"TestResource/Skill/Projectile/ChasingWaterBall/Ready.png");
}

ChasingWaterBallProjectile::~ChasingWaterBallProjectile()
{

}

void ChasingWaterBallProjectile::Update(float _dt)
{
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
	xValue.SetDuration(2.f);
	xValue.SetStartPoint(GetLocation().x + 50);
	xValue.SetEndPoint(GetLocation().x - 50);
	xValue.SetEasingEffect(EasingEffect::Linear);
	xValue.SetStepAnimation(StepAnimation::StepLoopPingPong);

	yValue.SetData(&y);
	yValue.SetDuration(2.f);
	yValue.SetStartPoint(GetLocation().y - 50);
	yValue.SetEndPoint(GetLocation().x + 50);
	yValue.SetEasingEffect(EasingEffect::InOutBounce);
	yValue.SetStepAnimation(StepAnimation::StepLoopPingPong);

}
