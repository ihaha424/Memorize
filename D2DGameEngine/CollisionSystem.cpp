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

bool CollisionSystem::IsComponentRegistered(PrimitiveComponent* component)
{
	CollisionProperty& collisionProperty = component->collisionProperty;
	PrimitiveComponentSet& set = collisionMap[collisionProperty.objectType];
	auto it = set.find(component);
	return it != set.end();
}

bool CollisionSystem::CheckCollisionShapeOverlapsMulti(
	std::vector<OverlapResult>& outOverlapResults, 
	const PrimitiveComponent* const self, 
	CollisionShape& collisionShape, 
	const Math::Vector2& pos, 
	const Math::Matrix& rotation, 
	const ECollisionChannel traceChannel, 
	const CollisionProperty& collisionProperty)
{
	bool hasOverlap = false;
	for (auto& [channel, set] : collisionMap)
	{
		if (collisionProperty.GetCollisionResponse(channel) == CollisionResponse::Ignore)
			continue;

		for (PrimitiveComponent* other : set)
		{
			if (other == self || !other->IsCollisionEnabled())
			{
				continue;
			}

			OverlapResult overlapResult;
			if (other->CheckOverlapComponent(overlapResult, pos, rotation, collisionShape, traceChannel, collisionProperty)) {
				outOverlapResults.push_back(overlapResult);
				hasOverlap = true;
			}
		}
	}

	return hasOverlap;
}

bool CollisionSystem::CheckCollisionShapeOverlapsMultiOnChannel(
	std::vector<struct OverlapResult>& outOverlapResults,
	const class PrimitiveComponent* const self,
	struct CollisionShape& collisionShape,
	const Math::Vector2& pos,
	const Math::Matrix& rotation,
	ECollisionChannel traceChannel,
	const CollisionProperty& collisionProperty,
	ECollisionChannel collisionChannel) 
{
	PrimitiveComponentSet& set = collisionMap[collisionChannel];

	bool hasOverlap = false;
	for (PrimitiveComponent* other : set) {
		if (other == self || !other->IsCollisionEnabled())
		{
			continue;
		}

		OverlapResult overlapResult;
		if (other->CheckOverlapComponent(overlapResult, pos, rotation, collisionShape, traceChannel, collisionProperty)) {
			outOverlapResults.push_back(overlapResult);
			hasOverlap = true;
		}
	}

	return hasOverlap;
}

bool CollisionSystem::CheckCollsionShapeSweepMulti(
	std::vector<HitResult>& outHitResults, 
	const class PrimitiveComponent* const self,
	CollisionShape& collisionShape, 
	const Math::Vector2& start, 
	const Math::Vector2& end, 
	const Math::Matrix& rotation, 
	ECollisionChannel traceChannel,
	const CollisionProperty& collisionProperty
) {

	bool hasBlockingHit = false;
	bool hasHit = false;
	for (auto& [channel, set] : collisionMap) 
	{
		if (collisionProperty.GetCollisionResponse(channel) == CollisionResponse::Ignore)
			continue;

		for (PrimitiveComponent* other : set) 
		{
			if (other == self || !other->IsCollisionEnabled()) 
			{
				continue;
			}

			HitResult hitResult;
			if (other->CheckSweepComponent(hitResult, start, end, rotation, collisionShape, traceChannel, collisionProperty)) {
				outHitResults.push_back(hitResult);
				hasHit = true;
				hasBlockingHit |= hitResult.bBlockingHit;
			}
		}
	}

	if (hasHit) 
	{
		std::sort(
			std::begin(outHitResults),
			std::end(outHitResults),
			[](const HitResult& lhs, const HitResult& rhs) {
				return lhs.time < rhs.time;
			}
		);
	}

	return hasBlockingHit;
}

bool CollisionSystem::CheckCollisionSweepMultiOnChannel(
	std::vector<HitResult>& outHitResults,
	const class PrimitiveComponent* const self,
	CollisionShape& collisionShape,
	const Math::Vector2& start,
	const Math::Vector2& end,
	const Math::Matrix& rotation,
	ECollisionChannel traceChannel,
	const CollisionProperty& collisionProperty,
	ECollisionChannel collisionChannel
) {

	PrimitiveComponentSet& set = collisionMap[collisionChannel];

	bool hasHit = false;
	for (PrimitiveComponent* other : set) {
		if (other == self || !other->IsCollisionEnabled()) {
			continue;
		}

		HitResult hitResult;
		if (other->CheckSweepComponent(hitResult, start, end, rotation, collisionShape, traceChannel, collisionProperty)) {
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

