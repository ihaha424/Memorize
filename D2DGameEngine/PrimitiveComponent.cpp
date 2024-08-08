#include "PrimitiveComponent.h"

#include "World.h"

PrimitiveComponent::PrimitiveComponent(Actor* _owner) : SceneComponent(_owner) {}

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
