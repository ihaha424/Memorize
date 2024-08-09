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
	UnregisterComponent(component);
	RegisterComponent(component);
}

bool CollisionSystem::CheckComponentOverlapsByChannel(
	std::vector<OverlapResult>& outOverlapResults, 
	PrimitiveComponent* primComp, 
	const Math::Vector2& pos,
	const Math::Matrix& rotation,
	ECollisionChannel channel) {
	if (!primComp->bCanCollide) return false;

	const CollisionProperty& collisionProperty = primComp->collisionProperty;
	PrimitiveComponentSet& set = collisionMap[channel];

	bool hasOverlap = false;
	for (PrimitiveComponent* other : set) {
		if (other == primComp || !other->IsCollisionEnabled()) {
			continue;
		}

		if (other->CheckComponentOverlapComponentWithResult(primComp, pos, rotation, outOverlapResults)) {
			hasOverlap = true;
		}
	}

	return hasOverlap;
}

bool CollisionSystem::CheckComponentSweepMultiByChannel(
	std::vector<HitResult>& outHitResults, 
	PrimitiveComponent* primComp, 
	const Math::Vector2& start, 
	const Math::Vector2& end, 
	const Math::Matrix& rotation,
	ECollisionChannel channel)
{
	if (!primComp->bCanCollide) return false;

	const CollisionProperty& collisionProperty = primComp->collisionProperty;
	PrimitiveComponentSet& set = collisionMap[channel];
	CollisionShape collisionShape;
	if (!primComp->GetCollisionShape(1.f, collisionShape)) return false;

	bool hasHit = false;
	for (PrimitiveComponent* other : set) {
		if (other == primComp || !other->IsCollisionEnabled()) {
			continue;
		}

		HitResult hitResult;
		if (other->CheckSweepComponent(hitResult, start, end, rotation, collisionShape, channel, collisionProperty)) {
			outHitResults.push_back(hitResult);
			hasHit = true;
		}
	}

	if (hasHit) {
		std::sort(
			std::begin(outHitResults),
			std::end(outHitResults),
			[](const HitResult& lhs, const HitResult& rhs) {
				return lhs.time < rhs.time;
			}
		);
	}

	return hasHit;
}

