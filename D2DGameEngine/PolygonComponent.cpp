#include "PolygonComponent.h"

#include "IntersectionUtil.h"

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
		res = intersectionUtil::PolygonBoxIntersectWithResult(myPolygon, otherBox, hitResult);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = otherCollisionShape.GetCapsuleHalfHeight(),
			.radius = otherCollisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::PolygonCapsuleIntersectWithResult(myPolygon, otherCapsule, hitResult);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = otherCollisionShape.GetCircleRadius()
		};
		res = intersectionUtil::PolygonCircleIntersectWithResult(myPolygon, otherCircle, hitResult);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
		for (Math::Vector2& point : otherPolygon.points) {
			point += pos;
			point = DXVec2::Transform(point, rotation);
		}
		res = intersectionUtil::PolygonPolygonIntersectWithResult(myPolygon, otherPolygon, hitResult);
	}	break;
	}

	if (res) {
		// Check collision type 
		bool bBlockingHit{ false };
		if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Block)
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
