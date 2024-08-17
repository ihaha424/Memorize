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

		//���� ���� ���� ������ 
		for (int i = 0; i < player->enemiesInRange.size(); i++)
		{
			Character* enemy = player->enemiesInRange[i];
			//�̹� �� ���� �������̸� �ѱ��.
			if (find(chasingEnemies.begin(), chasingEnemies.end(), enemy) != chasingEnemies.end())
				continue;

			//waterball���� �������� ���� �߰��Ѵ�.
			chasingEnemies.push_back(enemy);
			//�� projectile�� Ÿ������ �����Ѵ�. 
			target = enemy;
			//�������·� ��ȯ�Ѵ�. 
			state = State::Chase;
			break;
		}
	}
	else if (state == State::Chase)
	{
		//�̹��� ��ȯ 
		normalState->SetSprite(L"TestResource/Skill/Projectile/ChasingWaterBall/Chasing.png");
		normalState->SliceSpriteSheet(58, 56, 0, 0, 0, 0);

		//Ÿ���� ���� �̵�
		Math::Vector2 direction = target->GetLocation() - GetLocation();
		direction.Normalize();
		SetVelocity(direction, speed);

		//Ÿ���� ��ġ�� �̵������� ���� ���·�.
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
