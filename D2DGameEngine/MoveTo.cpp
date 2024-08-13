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
	toTarget.Normalize();

	// Move to 
	GetPawn()->SetVelocity(toTarget * /*TODO: How can I get movement speed*/ 500.f);
	
	// NOTE: Consider using Pathfinder instead
	// GetAIController()->MoveToLocation(target);
}

bool MoveToLocation::IsRunning() {

	float distanceSquared = (destination - GetPawn()->GetLocation()).LengthSquared();

	return distanceSquared > acceptableRadius * acceptableRadius;
}

void MoveToLocation::Run(float dt) {
	Math::Vector2 toDestination = destination - GetPawn()->GetLocation();

	float distanceSquared = toDestination.LengthSquared();
	if (distanceSquared <= acceptableRadius * acceptableRadius) return;

	toDestination.Normalize();

	GetPawn()->SetVelocity(toDestination * speed);
}
