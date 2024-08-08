#include "BoxComponent.h"

#include "IntersectionUtil.h"

bool BoxComponent::CheckComponentOverlapComponentImpl(PrimitiveComponent* primComp, const DXVec2& pos, const DXMat4x4& rotation)
{
	if (!bCanCollide || !primComp->bCanCollide) return false;
	if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Ignore) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);
	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	if (myCollisionShape.IsNearlyZero() || otherCollisionShape.IsNearlyZero())
		return false;

	// Build my Box
	Box myBox = Box::BuildAABB({0.f, 0.f}, boxExtent);
	myBox.ul = DXVec2::Transform(myBox.ul, GetWorldTransform());
	myBox.lr = DXVec2::Transform(myBox.lr, GetWorldTransform());

	bool res{ false };
	switch (otherCollisionShape.shapeType)
	{
	case ECollisionShape::Box: {
		// Build box from the collision shape and rotation
		Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
		otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
		otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
		res = intersectionUtil::BoxBoxIntersect(myBox, otherBox);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = otherCollisionShape.GetCapsuleHalfHeight(),
			.radius = otherCollisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::BoxCapsuleIntersect(myBox, otherCapsule);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = otherCollisionShape.GetCircleRadius()
		};
		res = intersectionUtil::BoxCircleIntersect(myBox, otherCircle);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
		for (Math::Vector2& point : otherPolygon.points) {
			point += pos;
			point = DXVec2::Transform(point, rotation);
		}
		res = intersectionUtil::BoxPolygonIntersect(myBox, otherPolygon);
	}	break;
	}

	return res;
}

bool BoxComponent::CheckComponentOverlapComponentWithResultImpl(PrimitiveComponent* primComp, const DXVec2& pos, const DXMat4x4& rotation, std::vector<OverlapResult>& outOverlap)
{
	if (!bCanCollide || !primComp->bCanCollide) return false;
	if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Ignore) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);
	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	if (myCollisionShape.IsNearlyZero() || otherCollisionShape.IsNearlyZero())
		return false;

	// Build my Box
	Box myBox = Box::BuildAABB({ 0.f, 0.f }, boxExtent);
	myBox.ul = DXVec2::Transform(myBox.ul, GetWorldTransform());
	myBox.lr = DXVec2::Transform(myBox.lr, GetWorldTransform());

	bool res{ false };
	HitResult hitResult;
	switch (otherCollisionShape.shapeType)
	{
	case ECollisionShape::Box: {
		// Build box from the collision shape and rotation
		Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
		otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
		otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
		res = intersectionUtil::BoxBoxIntersectWithResult(myBox, otherBox, hitResult);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = otherCollisionShape.GetCapsuleHalfHeight(),
			.radius = otherCollisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::BoxCapsuleIntersectWithResult(myBox, otherCapsule, hitResult);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = otherCollisionShape.GetCircleRadius()
		};
		res = intersectionUtil::BoxCircleIntersectWithResult(myBox, otherCircle, hitResult);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
		for (Math::Vector2& point : otherPolygon.points) {
			point += pos;
			point = DXVec2::Transform(point, rotation);
		}
		res = intersectionUtil::BoxPolygonIntersectWithResult(myBox, otherPolygon, hitResult);
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
