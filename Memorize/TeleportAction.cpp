#include "TeleportAction.h"

#include "D2DGameEngine\Pawn.h"
#include "D2DGameEngine\BehaviorTree.h"
#include "D2DGameEngine\AIController.h"

bool TeleportTo::IsRunning()
{
	return false;
}

void TeleportTo::Run(float dt)
{
	if (!bt->IsKeySet<Math::Vector2>(key)) {
		failed = true;
		return;
	};

	// Teleport to
	Math::Vector2 target = bt->GetKey<Math::Vector2>(key);
	Math::Vector2 currLocation = GetPawn()->GetLocation();
	Math::Vector2 teleportDelta = target - currLocation;
	GetPawn()->Teleport(teleportDelta);

	// NOTE: Consider using Pathfinder instead
	// GetAIController()->TeleportToLocation(target);
}
