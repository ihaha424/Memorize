#include "CollisionSystem.h"

#include "PrimitiveComponent.h"

#include "OverlapResult.h"
#include "HitResult.h"

void CollisionSystem::RegisterComponent(PrimitiveComponent* component) {

	CollisionProperty& collisionProperty = component->collisionProperty;
	PrimitiveComponentSet& set = collisionMap[collisionProperty.objectType];
	set.insert(component);
}

void CollisionSystem::UnregisterComponent(PrimitiveComponent* component)
{
	CollisionProperty& collisionProperty = component->collisionProperty;
	PrimitiveComponentSet& set = collisionMap[collisionProperty.objectType];
	auto it = set.find(component);
	if (it != set.end()) {
		set.erase(it);
	}
}

void CollisionSystem::ClearComponents()
{
	for (auto& [_, set] : collisionMap) {
		set.clear();
	}
}

void CollisionSystem::UpdateComponent(PrimitiveComponent* component)
{
	CollisionProperty& collisionProperty = component->collisionProperty;
	
	// Erase the previous record
	for (auto& [_, set] : collisionMap) {
		auto it = set.find(component);
		if (it != set.end()) {
			set.erase(it);
			break;
		}
	}

	// Map it with an updated property
	PrimitiveComponentSet& set = collisionMap[collisionProperty.objectType];
	set.insert(component);
}

bool CollisionSystem::CheckComponentOverlapsByChannel(
	std::vector<OverlapResult>& outOverlapResults, 
	PrimitiveComponent* primComp, 
	const Math::Vector2& pos) {
	if (!primComp->bCanCollide) return false;

	const CollisionProperty& collisionProperty = primComp->collisionProperty;
	PrimitiveComponentSet& set = collisionMap[collisionProperty.objectType];

	for (PrimitiveComponent* other : set) {
		if (other->bCanCollide) {
			/*CollisionShape otherShape = other->GetCollisionShape();
			if (otherShape.IsNearlyZero()) continue;

			std::vector<OverlapResult> overlapResults;
			if (other->CheckComponentOverlapComponentWithResult(primComp, pos, overlapResults)) {

			}*/
		}
		
	}

	return false;
}

