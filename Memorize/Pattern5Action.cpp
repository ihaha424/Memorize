#include "Pattern5Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"
#include "Boss.h"
#include "Player.h"
#include "BossChasingBomb.h"

bool Pattern5Action::IsRunning()
{
	if (!bt->IsKeySet<Math::Vector2>(destinationKey)) return false;

	if (elapsedTime >= patternInterval)
	{
		elapsedTime = 0.f;
		GetBehaviorTree()->GetKey<Boss*>("Boss")->Periodic_Pattern_Cool_Time = GetCooldown();
		return false;
	}
	else
	{
		// Get the target
		Math::Vector2 target = bt->GetKey<Math::Vector2>(destinationKey);
		// Get distance to the target
		float distanceSquared = Math::Vector2::DistanceSquared(GetPawn()->GetLocation(), target);

		if (distanceSquared > acceptableRadius * acceptableRadius)
		{
			return true;
		}
		else
		{
			elapsedTime = 0.f;
			GetBehaviorTree()->GetKey<Boss*>("Boss")->Periodic_Pattern_Cool_Time = GetCooldown();
			return false;
		}
	}
}

void Pattern5Action::Run(float dt)
{
	Super::Run(dt);
	
	if (!started)
	{
		if (!bt->IsKeySet<Math::Vector2>(destinationKey)) {
			failed = true;
			return;
		};

		Boss* boss = bt->GetKey<Boss*>("Boss");

		Math::Vector2 currLocation = GetPawn()->GetLocation();
		Math::Vector2 target = bt->GetKey<Math::Vector2>(destinationKey);
		float distanceSquared = Math::Vector2::DistanceSquared(currLocation, target);

		if (distanceSquared <= acceptableRadius * acceptableRadius) return;

		Math::Vector2 toTarget = target - currLocation;
		float toTargetDistanceSquared = toTarget.LengthSquared();
		toTarget.Normalize();

		Math::Vector2 velocity = toTarget * speed;
		Math::Vector2 velocityDelta = velocity * FIXED_RATE;
		float nextVelocityDeltaLengthSquared = velocityDelta.LengthSquared();

		// 남은 거리보다 다음 델타 이동거리가 더 길면
		// 오브젝트가 위치를 정확히 맞출 때까지 계속 왔다갔다 하는 
		// 상황이 나올 수 있으므로 속력을 조절합니다.
		if (toTargetDistanceSquared < nextVelocityDeltaLengthSquared)
		{
			float ratio = toTargetDistanceSquared / nextVelocityDeltaLengthSquared;
			velocity /= ratio;
		}

		// Move to 
		GetPawn()->SetVelocity(velocity);

		// Casting skill
		castingInterval -= dt;
		if (castingInterval <= 0.f)
		{
			Math::Vector2 bossLocation = boss->GetLocation();

			ChasingBomb* bomb = bt->GetWorld()->GetCurLevel()->CreateActor<ChasingBomb>();
			bomb->SetLocation(bossLocation.x, bossLocation.y);

			castingInterval += 1.f;
		}

		started = true;
	}
}
