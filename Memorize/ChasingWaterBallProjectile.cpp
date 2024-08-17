#include "ChasingWaterBallProjectile.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"
#include "Player.h"

std::vector<class Actor*> ChasingWaterBallProjectile::chasingEnemies{};

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


	//�� ���� ���� ���� �ݶ��̴�
	box->bGenerateOverlapEvent = false; //�� ������ ���� �ڽ��� ���� 
	rangeCircle = CreateComponent<CircleComponent>();
	rangeCircle->SetCircleRadius(range);
	rangeCircle->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapPawn);
	rangeCircle->SetCollisionObjectType(ECollisionChannel::PlayerProjectile);
	rangeCircle->bGenerateOverlapEvent = true;
	rootComponent->AddChild(rangeCircle);

	Inactivate();
	bIsPassable = false;
	bCollideWithOtherAttack = true;
	bHasEnding = false;
	endingTime = 1.f;
	
}

ChasingWaterBallProjectile::~ChasingWaterBallProjectile()
{

}

void ChasingWaterBallProjectile::OnOverlap(Actor* other, const OverlapInfo& overlap)
{
	std::cout << "overlap!" << std::endl;
	__super::OnOverlap(other, overlap);

	if (state == State::Idle)
	{
		
		//�̹� �� ���� �������̸� �ѱ��.
		if (find(chasingEnemies.begin(), chasingEnemies.end(), other) != chasingEnemies.end())
			return;

		//waterball���� �������� ���� �߰��Ѵ�.
		chasingEnemies.push_back(other);
		//�� projectile�� Ÿ������ �����Ѵ�. 
		target = other;
		//�������·� ��ȯ�Ѵ�. 
		state = State::Chase;
	}

}

void ChasingWaterBallProjectile::Update(float _dt)
{
	__super::Update(_dt);

	if (state == State::Idle)
	{
		rangeCircle->SetStatus(OS_ACTIVE);
		SetLocation(player->GetLocation().x + x, player->GetLocation().y + y);
	}
	else if (state == State::Chase)
	{
		rangeCircle->SetStatus(OS_INACTIVE);
		box->bGenerateOverlapEvent = true;
		//Ÿ���� ���� �̵�
		Math::Vector2 direction = target->GetLocation() - GetLocation();
		direction.Normalize();
		SetVelocity(direction, speed);

		//Ÿ���� ��ġ�� �̵������� ���� ���·�.
		float distance = Math::Vector2::Distance(GetLocation(), target->GetLocation());
		if (bEnding == true)
		{
			state = State::Boom;
		}
	}
	else if(state == State::Boom && bEnding == false)
	{
		bEnding = true;
		anim->SetState(endingState);
		mv->SetSpeed(0);
		elapsedTime = duration + delay;
	}
}

void ChasingWaterBallProjectile::Initialize()
{
	__super::Initialize();
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
