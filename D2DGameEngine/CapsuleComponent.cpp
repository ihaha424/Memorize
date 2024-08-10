#include "CapsuleComponent.h"

#include "D2DRenderer.h"

#include "IntersectionUtil.h"

bool CapsuleComponent::CheckSweepComponent(HitResult& outHit, const DXVec2& start, const DXVec2& end, const DXMat4x4& rotation, const CollisionShape& collisionShape, const ECollisionChannel collisionChannel, const CollisionProperty& collisionProperty)
{
	outHit = {};
	// Early out if collision is not enabled
	if (!bCanCollide) return false;

	if (GetCollisionObjectType() != collisionChannel) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);

	if (myCollisionShape.IsNearlyZero() || collisionShape.IsNearlyZero())
		return false;

	// Build my Capsule
	Math::Matrix myWorldTransform = GetWorldTransform();
	Math::Matrix myRotation = Math::ExtractRotation(myWorldTransform);
	Capsule myCapsule{
		.center = GetComponentLocation(),
		.direction = DXVec2::Transform(DXVec2::UnitY, myRotation),
		.extent = GetScaledCapsuleHalfHeight(),
		.radius = GetScaledCapsuleRadius()
	};

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
			res = intersectionUtil::BoxCapsuleIntersectWithResult(otherBox, myCapsule, outHit);
		}	break;
		case ECollisionShape::Capsule: {
			// Build Capsule from the collision shape and rotation
			Capsule otherCapsule{
				.center = pos,
				.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
				.extent = collisionShape.GetCapsuleHalfHeight(),
				.radius = collisionShape.GetCapsuleRadius()
			};
			res = intersectionUtil::CapsuleCapsuleIntersectWithResult(otherCapsule, myCapsule, outHit);
		}	break;
		case ECollisionShape::Circle: {
			// Build Circle from the collision shape and rotation
			Circle otherCircle{
				.center = pos,
				.radius = collisionShape.GetCircleRadius()
			};
			res = intersectionUtil::CircleCapsuleIntersectWithResult(otherCircle, myCapsule, outHit);
		}	break;
		case ECollisionShape::Polygon: {
			// Build Polygon from the collision shape and rotation
			TPolygon otherPolygon{ std::move(collisionShape.GetPolygonVertices()) };
			for (Math::Vector2& point : otherPolygon.points) {
				point += pos;
				point = DXVec2::Transform(point, rotation);
			}
			res = intersectionUtil::PolygonCapsuleIntersectWithResult(otherPolygon, myCapsule, outHit);
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
		// outHit.bStartPenetrating = true;
		outHit.hitComponent = this;

		outHit.location = pos;
		outHit.distance = (pos - start).Length();

		outHit.traceStart = start;
		outHit.traceEnd = end;

		outHit.time = outHit.distance / deltaSize;
	}

	return hasHit;
}

void CapsuleComponent::Render(D2DRenderer* _renderer)
{
	_renderer->PushTransform(GetWorldTransform());

#ifndef NDEBUG
	_renderer->DrawCapsule(
		{ 0, 0 }, capsuleHalfHeight, capsuleRadius,
		D2D_Color::Red
	);
#endif

	_renderer->PopTransform();
}

bool CapsuleComponent::CheckComponentOverlapComponentImpl(PrimitiveComponent* primComp, const DXVec2& pos, const DXMat4x4& rotation)
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
	Math::Matrix myWorldTransform = GetWorldTransform();
	Math::Matrix myRotation = Math::ExtractRotation(myWorldTransform);
	Capsule myCapsule {
		.center = GetComponentLocation(),
		.direction = DXVec2::Transform(DXVec2::UnitY, myRotation),
		.extent = GetScaledCapsuleHalfHeight(),
		.radius = GetScaledCapsuleRadius()
	};

	bool res{ false };
	switch (otherCollisionShape.shapeType)
	{
	case ECollisionShape::Box: {
		// Build box from the collision shape and rotation
		Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
		otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
		otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
		res = intersectionUtil::BoxCapsuleIntersect(otherBox, myCapsule);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = otherCollisionShape.GetCapsuleHalfHeight(),
			.radius = otherCollisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::CapsuleCapsuleIntersect(myCapsule, otherCapsule);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = otherCollisionShape.GetCircleRadius()
		};
		res = intersectionUtil::CircleCapsuleIntersect(otherCircle, myCapsule);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
		for (Math::Vector2& point : otherPolygon.points) {
			point += pos;
			point = DXVec2::Transform(point, rotation);
		}
		res = intersectionUtil::CapsulePolygonIntersect(myCapsule, otherPolygon);
	}	break;
	}

	return res;
}

bool CapsuleComponent::CheckComponentOverlapComponentWithResultImpl(PrimitiveComponent* primComp, const DXVec2& pos, const DXMat4x4& rotation, std::vector<OverlapResult>& outOverlap)
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
	Math::Matrix myWorldTransform = GetWorldTransform();
	Math::Matrix myRotation = Math::ExtractRotation(myWorldTransform);
	Capsule myCapsule{
		.center = GetComponentLocation(),
		.direction = DXVec2::Transform(DXVec2::UnitY, myRotation),
		.extent = GetScaledCapsuleHalfHeight(),
		.radius = GetScaledCapsuleRadius()
	};

	bool res{ false };
	HitResult hitResult;
	switch (otherCollisionShape.shapeType)
	{
	case ECollisionShape::Box: {
		// Build box from the collision shape and rotation
		Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
		otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
		otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
		res = intersectionUtil::BoxCapsuleIntersectWithResult(otherBox, myCapsule, hitResult);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = otherCollisionShape.GetCapsuleHalfHeight(),
			.radius = otherCollisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::CapsuleCapsuleIntersectWithResult(otherCapsule, myCapsule, hitResult);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = otherCollisionShape.GetCircleRadius()
		};
		res = intersectionUtil::CircleCapsuleIntersectWithResult(otherCircle, myCapsule, hitResult);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
		for (Math::Vector2& point : otherPolygon.points) {
			point += pos;
			point = DXVec2::Transform(point, rotation);
		}
		res = intersectionUtil::PolygonCapsuleIntersectWithResult(otherPolygon, myCapsule, hitResult);
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

		bool bStartPenetration{ false };
		if (currentlyOverlappingComponents.find(primComp) == currentlyOverlappingComponents.end()) 
		{
			bStartPenetration = true;
		}

		OverlapResult overlapResult{
			.bBlockingHit = bBlockingHit,
			.component = primComp
		};

		outOverlap.push_back(overlapResult);
	}

	return res;
}
