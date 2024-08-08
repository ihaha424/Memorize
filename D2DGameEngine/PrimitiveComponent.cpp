#include "PrimitiveComponent.h"

#include "World.h"

#include "IntersectionUtil.h"

PrimitiveComponent::PrimitiveComponent() {}

PrimitiveComponent::~PrimitiveComponent() {}

void PrimitiveComponent::UpdateOverlaps() {
	OverlappingComponentSet newOverlappingComponents;

	// Get nearby components
	


	// Swap the overlapping components.
	previouslyOverlappingComponents = currentlyOverlappingComponents;
	currentlyOverlappingComponents.clear();

	// TODO:


	// Check begin overlap
	for (auto& [otherComponent, hitResult] : currentlyOverlappingComponents) {
		auto it = previouslyOverlappingComponents.find(otherComponent);
		if (it == previouslyOverlappingComponents.end()) {
			if (bGenerateOverlapEvent) {
				// TODO: Notify overlap event
			}
			//if (_notify) _notify->OnBeginOverlap(this, otherComponent, hitResult);
		}
	}
	// Check end overlap
	for (auto& [otherComponent, hitResult] : previouslyOverlappingComponents) {
		auto it = currentlyOverlappingComponents.find(otherComponent);
		if (it == currentlyOverlappingComponents.end()) {
			if (bGenerateOverlapEvent) {
				// TODO: Notify overlap event
			}
			//if (_notify) _notify->OnEndOverlap(this, otherComponent, hitResult);
		}
	}
}

bool PrimitiveComponent::CheckComponentOverlapComponentImpl(
	PrimitiveComponent* primComp,
	const DXVec2& pos,
	const DXMat4x4& rotation) {
	if (!bCanCollide || !primComp->bCanCollide) return false;
	if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Ignore) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);
	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	if (myCollisionShape.IsNearlyZero() || otherCollisionShape.IsNearlyZero())
		return false;

	if (myCollisionShape.shapeType == ECollisionShape::Box) {
		// Build my Box
		Box myBox = Box::BuildAABB({ 0.f, 0.f }, boxExtent);
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
	else if (myCollisionShape.shapeType == ECollisionShape::Capsule) {

	}
	else if (myCollisionShape.shapeType == ECollisionShape::Circle) {

	}
	else if (myCollisionShape.shapeType == ECollisionShape::Polygon) {

	}

	return false;
}

bool PrimitiveComponent::CheckComponentOverlapComponentWithResultImpl(
	PrimitiveComponent* component,
	const DXVec2& pos, const DXMat4x4& rotation,
	std::vector<OverlapResult>& outOverlap) {



}
