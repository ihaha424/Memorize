#include "ChasingWaterBallProjectile.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"
#include "MovementComponent.h"
#include "Player.h"

std::vector<class Character*> ChasingWaterBallProjectile::chasingEnemies{};

ChasingWaterBallProjectile::ChasingWaterBallProjectile(World* _world)
	: Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Skill/Projectile/ChasingWaterBall/Ready.png");
	//normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->SliceSpriteSheet(58, 56, 0, 0, 0, 0);
	//normalState->FrameResize(73);
	//normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);

	endingState->SetSprite(L"TestResource/Skill/Projectile/ChasingWaterBall/Boom.png");
	//normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	endingState->SliceSpriteSheet(194, 181, 0, 0, 0, 0);
	//normalState->FrameResize(73);
	//normalState->SetFrameDurations({ 0.05f });

	Inactivate();
	bIsPassable = false;
	bCollideWithOtherAttack = true;
	bHasEnding = true;
	endingTime = 1.f;

}

ChasingWaterBallProjectile::~ChasingWaterBallProjectile()
{

}

void ChasingWaterBallProjectile::Update(float _dt)
{
	__super::Update(_dt);

	if (state == State::Idle)
	{
		SetLocation(player->GetLocation().x + x, player->GetLocation().y + y);

		//범위 내에 적이 있으면 
		for (int i = 0; i < player->enemiesInRange.size(); i++)
		{
			Character* enemy = player->enemiesInRange[i];
			//이미 이 적을 추적중이면 넘긴다.
			if (find(chasingEnemies.begin(), chasingEnemies.end(), enemy) != chasingEnemies.end())
				continue;

			//waterball들이 추적중이 적에 추가한다.
			chasingEnemies.push_back(enemy);
			//이 projectile의 타겟으로 설정한다. 
			target = enemy;
			//추적상태로 전환한다. 
			state = State::Chase;
			break;
		}
	}
	else if (state == State::Chase)
	{
		//이미지 전환 
		normalState->SetSprite(L"TestResource/Skill/Projectile/ChasingWaterBall/Chasing.png");
		normalState->SliceSpriteSheet(58, 56, 0, 0, 0, 0);

		//타겟을 향해 이동
		Math::Vector2 direction = target->GetLocation() - GetLocation();
		direction.Normalize();
		SetVelocity(direction, speed);

		//타겟의 위치로 이동했으면 폭발 상태로.
		float distance = Math::Vector2::Distance(GetLocation(), target->GetLocation());
		if (distance < 5)
		{
			state = State::Boom;
		}
	}
	else
	{
		bEnding = true;
		anim->SetState(endingState);
		mv->SetSpeed(0);
	}
}

void ChasingWaterBallProjectile::Initialize()
{
	//xValue.SetData(&x);
	//xValue.SetDuration(1.f);
	//xValue.SetStartPoint(GetLocation().x + 100);
	//xValue.SetEndPoint(GetLocation().x - 100);
	//xValue.SetEasingEffect(EasingEffect::Linear);
	//xValue.SetStepAnimation(StepAnimation::StepLoopPingPong);
	//
	//yValue.SetData(&y);
	//yValue.SetDuration(0.5f);
	//yValue.SetStartPoint(GetLocation().y - 5);
	//yValue.SetEndPoint(GetLocation().y + 5);
	//yValue.SetEasingEffect(EasingEffect::InOutBounce);
	//yValue.SetStepAnimation(StepAnimation::StepLoopPingPong);

}
