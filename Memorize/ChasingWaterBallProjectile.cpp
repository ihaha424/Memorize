#include "ChasingWaterBallProjectile.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/PolygonComponent.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/RandomGenerator.h"
#include "MovementComponent.h"
#include "Player.h"
#include "Boss.h"
#include "Bat.h"
#include "Scarecrow.h"

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


	box->SetPolygon({ {-80, -150}, {80, -150}, {80, 150}, {-80, 150} });
	box->bGenerateOverlapEvent = false; //적 감지를 위해 박스는 꺼줌 

	bIsPassable = false;
	bCollideWithOtherAttack = true;
	bHasEnding = true;
	endingTime = 1.f;
	
	prevPos = GetLocation();
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

void ChasingWaterBallProjectile::Update(float _dt)
{
	__super::Update(_dt);

	if (state == State::Idle)
	{
		
		SetLocation(player->GetLocation().x + x, player->GetLocation().y + y);

		if (elapsedTime < chaseDelay)
			return;


		Boss* boss = GetWorld()->FindActorByType<Boss>();
		if(boss)
			chasingEnemies.push_back(boss);

		Scarecrow* scarecrow = GetWorld()->FindActorByType<Scarecrow>();
		if (scarecrow)
			chasingEnemies.push_back(scarecrow);

		std::vector<Bat*> bats = GetWorld()->FindAllActorsByType<Bat>();
		for (auto bat : bats)
		{
			if(bat->GetStatus() == OS_ACTIVE)
				chasingEnemies.push_back(bat);
		}

		float minDistance = 999999;
		//가장 가까운 적의 위치를 타겟으로 잡는다. 
		for (auto enemy : chasingEnemies)
		{
			float enemyDistance = Math::Vector2::Distance(GetLocation(), enemy->GetLocation());
			if (enemyDistance < minDistance)
			{
				minDistance = enemyDistance;
				targetPos = enemy->GetLocation();
			}
		}

		//추적할 적이 있으면 
		if (minDistance < 999999)
		{
			//제어점 설정
			Math::Vector2 direction = targetPos - GetLocation();
			int rand = Random::Get<int>(1);
			if(direction.x > direction.y)
				if(rand == 0)
					controlPoint = GetLocation() + (direction * 0.7f) + Math::Vector2(0, 400);
				else
					controlPoint = GetLocation() + (direction * 0.7f) + Math::Vector2(0, -400);
			else
				if(rand == 0)
					controlPoint = GetLocation() + (direction * 0.7f) + Math::Vector2(400, 0);
				else
					controlPoint = GetLocation() + (direction * 0.7f) + Math::Vector2(-400, 0);

			startPos = GetLocation();
			state = State::Chase;
			anim->SetState(chaseState);
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
		Math::Vector2 pos = CalculateBezierPoint(t, startPos, controlPoint, targetPos);
		SetLocation(pos.x, pos.y);

		t += _dt * 0.8;

		if (t >= 1.0f)
		{
			state = State::Boom;
		}

		//방향에 맞게 회전
		Math::Vector2 direction = GetLocation() - prevPos;
		direction.Normalize();
		double rotateRad = std::acos(direction.Dot(Math::Vector2(1.f, 0.f)));
		if (direction.y < 0)
			rotateRad *= -1;
		rootComponent->SetRotation(rotateRad * 180.f / PI - 90);
		prevPos = GetLocation();

		elapsedTime = 0.f;
	}
	else if(state == State::Boom && bEnding == false)
	{
		bEnding = true;
		anim->SetState(endingState);
		SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_ChasingWaterBall02.wav");
		mv->SetSpeed(0);
		elapsedTime = duration + delay;
	}
	else if (mv->GetStatus() == OS_INACTIVE)
	{
		Destroy();
	}
}

Math::Vector2 ChasingWaterBallProjectile::CalculateBezierPoint(float t, const Math::Vector2& p0, const Math::Vector2& p1, const Math::Vector2& p2)
{
	float u = 1.0f - t;
	float tt = t * t;
	float uu = u * u;

	Math::Vector2 p = uu * p0; // (1 - t)^2 * p0
	p += 2 * u * t * p1; // 2(1 - t)t * p1
	p += tt * p2; // t^2 * p2

	return p;
}