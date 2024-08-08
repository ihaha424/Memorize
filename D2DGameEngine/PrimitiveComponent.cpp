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

bool PrimitiveComponent::CheckComponentOverlapComponentImpl(PrimitiveComponent* primComp, const DXVec2& pos)
{
	if (!bCanCollide || !primComp->bCanCollide) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);
	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	if (myCollisionShape.IsNearlyZero() || otherCollisionShape.IsNearlyZero())
		return false;

	if (myCollisionShape.shapeType == ECollisionShape::Box) {
		// Box myBox = Box::BuildAABB(GetComponentLocation(), myCollisionShape.GetExtent())

		switch (otherCollisionShape.shapeType)
		{
		case ECollisionShape::Box: {

		}	break;
		case ECollisionShape::Capsule: {

		}	break;
		case ECollisionShape::Circle: {

		}	break;
		case ECollisionShape::Polygon: {

		}	break;
		}
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
	PrimitiveComponent* component, const DXVec2& pos, 
	std::vector<OverlapResult>& outOverlap) {



}
