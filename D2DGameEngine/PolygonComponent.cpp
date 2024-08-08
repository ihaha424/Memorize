#include "PolygonComponent.h"

#include "IntersectionUtil.h"

bool PolygonComponent::CheckSweepComponent(HitResult& outHit, const DXVec2& start, const DXVec2& end, const DXMat4x4& rotation, const CollisionShape& collisionShape, const ECollisionChannel collisionChannel, const CollisionProperty& collisionProperty)
{
	outHit = {};
	// Early out if collision is not enabled
	if (!bCanCollide) return false;

	if (GetCollisionObjectType() != collisionChannel) return false;

	// Build my Capsule
	TPolygon myPolygon{ vertices };
	for (Math::Vector2& point : myPolygon.points) {
		point = DXVec2::Transform(point, GetWorldTransform());
	}

	const DXVec2 delta = end - start;
	DXVec2 deltaDir = delta; deltaDir.Normalize();
	float deltaSize = delta.Length();

	constexpr uint maxNumSteps = 20;

	float stepSize = 1.f;
	uint numSteps = deltaSize / stepSize;
	if (maxNumSteps < numSteps) {
		stepSize = deltaSize / maxNumSteps;
		numSteps = maxNumSteps;
	}

	bool hasHit{ false };
	DXVec2 pos{ start };
	for (uint step = 0; step <= numSteps; ++step) {
		bool res{ false };
		switch (collisionShape.shapeType)
		{
		case ECollisionShape::Box: {
			// Build box from the collision shape and rotation
			Box otherBox = Box::BuildAABB(pos, collisionShape.GetExtent());
			otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
			otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
			res = intersectionUtil::BoxPolygonIntersectWithResult(otherBox, myPolygon, outHit);
		}	break;
		case ECollisionShape::Capsule: {
			// Build Capsule from the collision shape and rotation
			Capsule otherCapsule{
				.center = pos,
				.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
				.extent = collisionShape.GetCapsuleHalfHeight(),
				.radius = collisionShape.GetCapsuleRadius()
			};
			res = intersectionUtil::CapsulePolygonIntersectWithResult(otherCapsule, myPolygon, outHit);
		}	break;
		case ECollisionShape::Circle: {
			// Build Circle from the collision shape and rotation
			Circle otherCircle{
				.center = pos,
				.radius = collisionShape.GetCircleRadius()
			};
			res = intersectionUtil::CirclePolygonIntersectWithResult(otherCircle, myPolygon, outHit);
		}	break;
		case ECollisionShape::Polygon: {
			// Build Polygon from the collision shape and rotation
			TPolygon otherPolygon{ std::move(collisionShape.GetPolygonVertices()) };
			for (Math::Vector2& point : otherPolygon.points) {
				point += pos;
				point = DXVec2::Transform(point, rotation);
			}
			res = intersectionUtil::PolygonPolygonIntersectWithResult(otherPolygon, myPolygon, outHit);
		}	break;
		}

		pos += deltaDir * stepSize;
		hasHit = res;
		if (hasHit) break;
	}

	if (hasHit) {
		// Check collision type 
		bool bBlockingHit{ false };
		if (collisionProperty.GetCollisionResponse(this->GetCollisionObjectType()) == CollisionResponse::Block &&
			this->collisionProperty.GetCollisionResponse(collisionProperty.objectType) == CollisionResponse::Block)
		{
			bBlockingHit = true;
		}

		outHit.bBlockingHit = bBlockingHit;
		outHit.bStartPenetrating = true;
		outHit.hitComponent = this;

		outHit.location = pos;
		outHit.distance = (pos - start).Length();

		outHit.traceStart = start;
		outHit.traceEnd = end;

		outHit.time = outHit.distance / deltaSize;
	}

	return false;
}

bool PolygonComponent::CheckComponentOverlapComponentImpl(PrimitiveComponent* primComp, const DXVec2& pos, const DXMat4x4& rotation)
{
	if (!bCanCollide || !primComp->bCanCollide) return false;
	if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Ignore) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);
	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	if (myCollisionShape.IsNearlyZero() || otherCollisionShape.IsNearlyZero())
		return false;

	// Build my Polygon
	TPolygon myPolygon{ vertices };
	for (Math::Vector2& point : myPolygon.points) {
		point = DXVec2::Transform(point, GetWorldTransform());
	}

	bool res{ false };
	switch (otherCollisionShape.shapeType)
	{
	case ECollisionShape::Box: {
		// Build box from the collision shape and rotation
		Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
		otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
		otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
		res = intersectionUtil::BoxPolygonIntersect(otherBox, myPolygon);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = otherCollisionShape.GetCapsuleHalfHeight(),
			.radius = otherCollisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::CapsulePolygonIntersect(otherCapsule, myPolygon);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = otherCollisionShape.GetCircleRadius()
		};
		res = intersectionUtil::CirclePolygonIntersect(otherCircle, myPolygon);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
		for (Math::Vector2& point : otherPolygon.points) {
			point += pos;
			point = DXVec2::Transform(point, rotation);
		}
		res = intersectionUtil::PolygonPolygonIntersect(myPolygon, otherPolygon);
	}	break;
	}

	return res;
}

bool PolygonComponent::CheckComponentOverlapComponentWithResultImpl(PrimitiveComponent* primComp, const DXVec2& pos, const DXMat4x4& rotation, std::vector<OverlapResult>& outOverlap)
{
	if (!bCanCollide || !primComp->bCanCollide) return false;
	if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Ignore) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);
	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	if (myCollisionShape.IsNearlyZero() || otherCollisionShape.IsNearlyZero())
		return false;

	// Build my Capsule
	TPolygon myPolygon{ vertices };
	for (Math::Vector2& point : myPolygon.points) {
		point = DXVec2::Transform(point, GetWorldTransform());
	}

	bool res{ false };
	HitResult hitResult;
	switch (otherCollisionShape.shapeType)
	{
	case ECollisionShape::Box: {
		// Build box from the collision shape and rotation
		Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
		otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
		otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
		res = intersectionUtil::BoxPolygonIntersectWithResult(otherBox, myPolygon, hitResult);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = otherCollisionShape.GetCapsuleHalfHeight(),
			.radius = otherCollisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::CapsulePolygonIntersectWithResult(otherCapsule, myPolygon, hitResult);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = otherCollisionShape.GetCircleRadius()
		};
		res = intersectionUtil::CirclePolygonIntersectWithResult(otherCircle, myPolygon, hitResult);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
		for (Math::Vector2& point : otherPolygon.points) {
			point += pos;
			point = DXVec2::Transform(point, rotation);
		}
		res = intersectionUtil::PolygonPolygonIntersectWithResult(otherPolygon, myPolygon, hitResult);
	}	break;
	}

	if (res) {
		// Check collision type 
		bool bBlockingHit{ false };
		if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Block &&
			primComp->collisionProperty.GetCollisionResponse(GetCollisionObjectType()) == CollisionResponse::Block)
		{
			bBlockingHit = true;
		}

		OverlapResult overlapResult{
			.bBlockingHit = bBlockingHit,
			.component = primComp
		};

		outOverlap.push_back(overlapResult);
	}

	return res;
}
