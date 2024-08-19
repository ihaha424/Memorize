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
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_ChasingWaterBall03.png");
	normalState->SliceSpriteSheet(140, 140, 0, 0, 0, 0);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_ChasingWaterBall02.png");
	endingState->SliceSpriteSheet(200, 200, 0, 0, 0, 0);
	endingState->FrameResize(18);
	endingState->SetFrameDurations({ 0.05f });

	chaseState = anim->CreateState<AnimationState>();
	chaseState->SetSprite(L"TestResource/Player/Skill/Skill_ChasingWaterBall01.png");
	chaseState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	chaseState->SetFrameDurations({ 0.05f });


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
	__super::OnOverlap(other, overlap);

	if (overlap.overlapInfo.hitComponent->GetCollisionObjectType() == ECollisionChannel::Enemy)
	{
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
			anim->SetState(chaseState);
		}
	}



}

void ChasingWaterBallProjectile::Update(float _dt)
{
	__super::Update(_dt);

	std::cout << chasingEnemies.size() << std::endl;

	if (state == State::Idle)
	{
		rangeCircle->SetStatus(OS_ACTIVE);
		rangeCircle->bShouldOverlapTest = true;
		SetLocation(player->GetLocation().x + x, player->GetLocation().y + y);

		if (elapsedTime > duration)
			elapsedTime += endingTime;
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
		if (distance <= 5)
		{
			state = State::Boom;
			auto it = std::find(chasingEnemies.begin(), chasingEnemies.end(), target);
			if (it != chasingEnemies.end()) 
				chasingEnemies.erase(it);
			target = nullptr;
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
	target = nullptr;
	chasingEnemies.clear();
	state = State::Idle;
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
