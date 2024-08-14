#include "MoveTo.h"

#include "Pawn.h"
#include "BehaviorTree.h"
#include "AIController.h"

#include "Debug.h"

bool MoveTo::IsRunning() {
	// Run only when key is set.
	if (!bt->IsKeySet<Math::Vector2>(key)) return false;

	// Get the target
	Math::Vector2 target = bt->GetKey<Math::Vector2>(key);
	// Get distance to the target
	float distanceSquared = Math::Vector2::DistanceSquared(GetPawn()->GetLocation(), target);
	
	return distanceSquared > acceptableRadius * acceptableRadius;
}

void MoveTo::Run(float dt) {
	if (!bt->IsKeySet<Math::Vector2>(key)) {
		failed = true;
		return;
	};

	Math::Vector2 currLocation = GetPawn()->GetLocation();
	Math::Vector2 target = bt->GetKey<Math::Vector2>(key);
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
	
	// NOTE: Consider using Pathfinder instead
	// GetAIController()->MoveToLocation(target);
}

bool MoveToLocation::IsRunning() {
	Math::Vector2 currLocation = GetPawn()->GetLocation();
	float distanceSquared = (destination - currLocation).LengthSquared();

	return distanceSquared > acceptableRadius * acceptableRadius;
}

void MoveToLocation::Run(float dt) {
	Math::Vector2 currLocation = GetPawn()->GetLocation();
	Math::Vector2 toDestination = destination - currLocation;

	float toDestinationDistanceSquared = toDestination.LengthSquared();
	if (toDestinationDistanceSquared <= acceptableRadius * acceptableRadius) return;

	toDestination.Normalize();

	Math::Vector2 velocity = toDestination * speed;
	Math::Vector2 velocityDelta = velocity * FIXED_RATE;
	float nextVelocityDeltaLengthSquared = velocityDelta.LengthSquared();

	// 남은 거리보다 다음 델타 이동거리가 더 길면
	// 오브젝트가 위치를 정확히 맞출 때까지 계속 왔다갔다 하는 
	// 상황이 나올 수 있으므로 속력을 조절합니다.
	if (toDestinationDistanceSquared < nextVelocityDeltaLengthSquared)
	{
		float ratio = toDestinationDistanceSquared / nextVelocityDeltaLengthSquared;
		velocity /= ratio;
	}

	GetPawn()->SetVelocity(velocity);
}
