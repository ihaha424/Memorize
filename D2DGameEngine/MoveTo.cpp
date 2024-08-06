#include "MoveTo.h"

#include "GameObject.h"
#include "BehaviorTree.h"

#include "Debug.h"

bool MoveTo::IsRunning() {
	// TODO: Run only when key is set.
	// if (!myBT->IsKeySet<Vec2f>(_key)) return false;

	// TODO: Get the target
	// Vec2f target = myBT->GetKey<Vec2f>(_key);
	// TODO: Get distance to the target
	// float distance = Math2D::Distance(GetOwner()->GetWorldLocation(), target);
	
	// return distance > _acceptableRadius;
}

void MoveTo::Run(float dt) {
	/*if (!myBT->IsKeySet<Vec2f>(_key)) {
		failed = true;
		return;
	};

	Vec2f target = myBT->GetKey<Vec2f>(_key);
	float distance = Math2D::Distance(GetOwner()->GetWorldLocation(), target);

	LOG_VERBOSE(dbg::text("AI: Move To (", target.x, ", ", target.y, "), Range=", distance));
	if (distance <= _acceptableRadius) return;

	Vec2f toTarget = target - GetOwner()->GetWorldLocation();
	Vec2f dir = Math2D::Normalize(toTarget);*/

	/*Movement* movement = GetOwner()->GetMovementComponent();
	movement->AddInput(dir);*/
}
