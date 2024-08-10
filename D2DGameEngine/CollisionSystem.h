#pragma once

#include "framework.h"

#include "CollisionProperty.h"

#include "HitResult.h"

class CollisionSystem {

public:
	using PrimitiveComponentSet = std::unordered_set<class PrimitiveComponent*>;
	using CollisionMap = std::unordered_map<ECollisionChannel, PrimitiveComponentSet>;

	CollisionMap collisionMap;

	CollisionSystem() : collisionMap(GetNumCollisionChannels()) {
		for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
			collisionMap[GetCollisionChannelByIndex(i)] = PrimitiveComponentSet();
		}
	}

	void RegisterComponent(PrimitiveComponent* component);
	void UnregisterComponent(PrimitiveComponent* component);
	void ClearComponents();
	void UpdateComponent(PrimitiveComponent* component);
	bool IsComponentRegistered(PrimitiveComponent* component);

	bool CheckCollisionShapeOverlapsMulti(
		std::vector<struct OverlapResult>& outOverlapResults,
		const class PrimitiveComponent* const self,
		struct CollisionShape& collisionShape,
		const Math::Vector2& pos,
		const Math::Matrix& rotation,
		const ECollisionChannel traceChannel,
		const CollisionProperty& collisionProperty
	);

	bool CheckCollisionShapeOverlapsMultiOnChannel(
		std::vector<struct OverlapResult>& outOverlapResults,
		const class PrimitiveComponent* const self,
		struct CollisionShape& collisionShape,
		const Math::Vector2& pos,
		const Math::Matrix& rotation,
		ECollisionChannel traceChannel,
		const CollisionProperty& collisionProperty,
		ECollisionChannel collisionChannel
	);

	bool CheckCollsionShapeSweepMulti(
		std::vector<HitResult>& outHitResults,
		const class PrimitiveComponent* const self,
		struct CollisionShape& collisionShape,
		const Math::Vector2& start,
		const Math::Vector2& end,
		const Math::Matrix& rotation,
		const ECollisionChannel traceChannel,
		const CollisionProperty& collisionProperty
	);

	bool CheckCollisionSweepMultiOnChannel(
		std::vector<HitResult>& outHitResults,
		const class PrimitiveComponent* const self,
		struct CollisionShape& collisionShape,
		const Math::Vector2& start,
		const Math::Vector2& end,
		const Math::Matrix& rotation,
		ECollisionChannel traceChannel,
		const CollisionProperty& collisionProperty,
		ECollisionChannel collisionChannel
	);

};
