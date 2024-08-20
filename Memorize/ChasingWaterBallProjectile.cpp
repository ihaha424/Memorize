#include "ChasingWaterBallProjectile.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/PolygonComponent.h"
#include "../D2DGameEngine/World.h"
#include "MovementComponent.h"
#include "Player.h"
#include "Boss.h"
#include "Bat.h"

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


	//적 감지 위한 원형 콜라이더
	box->SetPolygon({ {-80, 150}, {80, -150}, {-80, -150}, {80, 150} });

	box->bGenerateOverlapEvent = false; //적 감지를 위해 박스는 꺼줌 
	//rangeCircle = CreateComponent<CircleComponent>();
	//rangeCircle->SetCircleRadius(range);
	//rangeCircle->SetCollisionObjectType(ECollisionChannel::PlayerProjectile);
	//rangeCircle->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	//rangeCircle->collisionProperty.SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Overlap);
	//rangeCircle->collisionProperty.SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Overlap);
	//rangeCircle->bGenerateOverlapEvent = true;
	//rootComponent->AddChild(rangeCircle);

	bIsPassable = false;
	bCollideWithOtherAttack = true;
	bHasEnding = true;
	endingTime = 1.f;
	

}

ChasingWaterBallProjectile::~ChasingWaterBallProjectile()
{
	
}

void ChasingWaterBallProjectile::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	state = State::Boom;
	bHasEnding = true;
}

//void ChasingWaterBallProjectile::OnOverlap(Actor* other, const OverlapInfo& overlap)
//{
//	__super::OnOverlap(other, overlap);
//
//	if (overlap.overlapInfo.hitComponent->GetCollisionObjectType() == ECollisionChannel::Enemy)
//	{
//		if (state == State::Idle)
//		{
//
//			//이미 이 적을 추적중이면 넘긴다.
//			if (find(chasingEnemies.begin(), chasingEnemies.end(), other) != chasingEnemies.end())
//				return;
//
//			//waterball들이 추적중인 적에 추가한다.
//			chasingEnemies.push_back(other);
//			//이 projectile의 타겟으로 설정한다. 
//			target = other;
//			//추적상태로 전환한다. 
//			state = State::Chase;
//			anim->SetState(chaseState);
//		}
//	}
//
//
//
//}

void ChasingWaterBallProjectile::Update(float _dt)
{
	__super::Update(_dt);

	
	if (state == State::Idle)
	{
		SetLocation(player->GetLocation().x + x, player->GetLocation().y + y);

		Boss* boss = GetWorld()->FindActorByType<Boss>();
		auto it = find(chasingEnemies.begin(), chasingEnemies.end(), boss);
		if (it == chasingEnemies.end())
		{
			if (Math::Vector2::Distance(boss->GetLocation(), GetLocation()) < range)
			{
				target = boss;
				state = State::Chase;
				chasingEnemies.push_back(target);
			}
			
		}
		else
		{
			std::vector<Bat*> bats = GetWorld()->FindAllActorsByType<Bat>();
			for (auto bat : bats)
			{
				it = find(chasingEnemies.begin(), chasingEnemies.end(), boss);
				if (it == chasingEnemies.end())
				{
					if (Math::Vector2::Distance(bat->GetLocation(), GetLocation()) < range)
					{
						target = bat;
						state = State::Chase;
						chasingEnemies.push_back(target);
						break;
					}

				}
			}
		}

		if (elapsedTime > duration)
		{
			elapsedTime += endingTime;
			bHasEnding = true;
		}
			
	}
	else if (state == State::Chase)
	{
		box->bGenerateOverlapEvent = true;
		//타겟을 향해 이동
		Math::Vector2 direction = target->GetLocation() - GetLocation();
		direction.Normalize();
		SetVelocity(direction, speed);
		elapsedTime = 0.f;
	}
	else if(state == State::Boom && bEnding == false)
	{
		bEnding = true;
		anim->SetState(endingState);
		mv->SetSpeed(0);
		elapsedTime = duration + delay;

		auto it = find(chasingEnemies.begin(), chasingEnemies.end(), target);
		if(it != chasingEnemies.end())
			chasingEnemies.erase(it);
	}
	else if (mv->GetStatus() == OS_INACTIVE)
	{
		Destroy();
		auto it = find(chasingEnemies.begin(), chasingEnemies.end(), target);
		if (it != chasingEnemies.end())
			chasingEnemies.erase(it);
	}
}
