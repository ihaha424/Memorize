#include "PrimitiveComponent.h"

#include "World.h"
#include "IntersectionUtil.h"
PrimitiveComponent::PrimitiveComponent(Actor* _owner) : SceneComponent(_owner) {}

PrimitiveComponent::~PrimitiveComponent() {}

bool PrimitiveComponent::CheckSweepComponent(
	HitResult& outHit, 
	const DXVec2& start, 
	const DXVec2& end, 
	const DXMat4x4& rotation, 
	const CollisionShape& collisionShape, 
	const ECollisionChannel collisionChannel,
	const CollisionProperty& collisionProperty)
{
	return false;
}

void PrimitiveComponent::BeginComponentOverlap(const OverlapInfo& otherOverlap, bool bDoNotifies)
{
	PrimitiveComponent* otherComp = otherOverlap.overlapInfo.hitComponent;

	Actor* otherActor = otherComp->GetOwner();
	Actor* myActor = GetOwner();

	if (myActor == otherActor) return;

	PushOverlappingComponent(otherComp, otherOverlap);

	if (bDoNotifies) {

		OnComponentBeginOverlap();
		otherComp->OnComponentBeginOverlap();

		// myActor->NotifyActorBeginOverlap(otherActor);
		// otherActor->NotifyActorBeginOverlap(otherActor);
	}
}

void PrimitiveComponent::EndComponentOverlap(const OverlapInfo& otherOverlap, bool bDoNotifies, bool bSkipNotifySelf)
{
	PrimitiveComponent* otherComp = otherOverlap.overlapInfo.hitComponent;
	if (otherComp == nullptr) return;

	Actor* otherActor = otherComp->GetOwner();
	Actor* myActor = GetOwner();

	if (myActor == otherActor) return;

	PopOverlappingComponent(otherComp);

	if (bDoNotifies) {

		if (!bSkipNotifySelf) {
			OnComponentEndOverlap();
		}

		otherComp->OnComponentEndOverlap();

		// myActor->NotifyActorEndOverlap(otherActor);
		// otherActor->NotifyActorEndOverlap(otherActor);
	}

}



void PrimitiveComponent::UpdateOverlaps(const std::vector<OverlapInfo>& overlaps, bool bDoNotifies = true) {
	
	OverlappingComponentSet previouslyOverlappingComponents{ std::move(currentlyOverlappingComponents) };
	currentlyOverlappingComponents.clear();

	// Check begin overlap
	for (const OverlapInfo& overlapInfo : overlaps) {
		PrimitiveComponent* other = overlapInfo.overlapInfo.hitComponent;
		if (this == other) continue;

		auto it = previouslyOverlappingComponents.find(other);
		if (it == previouslyOverlappingComponents.end()) {
			BeginComponentOverlap(overlapInfo, bDoNotifies);
		}
	}

	// Check end overlap
	for (auto& [otherComponent, overlapInfo] : previouslyOverlappingComponents) {
		auto it = currentlyOverlappingComponents.find(otherComponent);
		if (it == currentlyOverlappingComponents.end()) {
			if (bGenerateOverlapEvent) {
				EndComponentOverlap(overlapInfo, bDoNotifies);
			}
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
		Box myBox = Box::BuildAABB({ 0.f, 0.f }, myCollisionShape.GetExtent());
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

bool PrimitiveComponent::MoveComponentImpl(
	const DXVec2& delta, 
	bool bSweep, 
	HitResult* outHitResult)
{
	const Math::Vector2 traceStart = GetComponentLocation();
	const Math::Vector2 traceEnd = traceStart + delta;

	const bool bSkipPhysicsMove = !bSimulatePhysics;

	HitResult blockingHit{};
	blockingHit.bBlockingHit = false;
	blockingHit.time = 1.f;
	bool bMoved = false;
	bool bFilledHitResult = false;

	std::vector<OverlapInfo> pendingOverlaps;

	Actor* actor = GetOwner();

	if (!bSweep) {
		SetTranslation(traceEnd.x, traceEnd.y);
	}
	else {
		std::vector<HitResult> hits;

		DXVec2 newLocation = traceStart;

		const bool bCollisionEnabled = IsCollisionEnabled();
		World* myWorld = GetWorld();

		if (myWorld && bCollisionEnabled) {
			if (actor) {
				// Debug Message
			}
			else {
				// Debug Message
			}

			bool const bHadBlockingHit = myWorld->CheckComponentSweepMulti(hits, this, traceStart, traceEnd);

			if (hits.size() > 0) {
				const float deltaSize = delta.Length();
				for (uint i = 0; i < hits.size(); ++i) {
					PullBackHit(hits[i], traceStart, traceEnd, deltaSize);
				}
			}

			int firstNonInitialOverlapIndex = -1;
			if (bHadBlockingHit || bGenerateOverlapEvent) {
				int blockingHitIndex = -1;
				float blockingHitNormalDotDelta = (std::numeric_limits<float>::max)();

				for (int hitIndex = 0; hitIndex < hits.size(); hitIndex++) {
					const HitResult& testHit = hits[hitIndex];

					if (testHit.bBlockingHit) {
						if (testHit.bStartPenetrating) {
							const float normalDotDelta = testHit.impactNormal.Dot(delta);
							if (normalDotDelta < blockingHitNormalDotDelta) {
								blockingHitNormalDotDelta = normalDotDelta;
								blockingHitIndex = hitIndex;
							}
						}
						else if (blockingHitIndex == -1) {
							blockingHitIndex = hitIndex;
						}
					}
					else if (bGenerateHitEvent) {
						PrimitiveComponent* overlapComponent = testHit.hitComponent;

						if (overlapComponent && overlapComponent->bGenerateOverlapEvent) {
							if (blockingHitIndex >= 0 && testHit.time > hits[blockingHitIndex].time) {
								break;
							}

							if (firstNonInitialOverlapIndex == -1 && testHit.time > 0.f) {
								firstNonInitialOverlapIndex = pendingOverlaps.size();
							}
						}
					}
				}

				if (blockingHitIndex >= 0) {
					blockingHit = hits[blockingHitIndex];
					bFilledHitResult = true;
				}
			}

			if (!blockingHit.bBlockingHit) {
				newLocation = traceEnd;
			}
			else {
				// Check if bFilledHitResult
				newLocation = traceStart + (blockingHit.time * (traceEnd - traceStart));
			}
		}
		else if (delta.LengthSquared() > 0.f) {
			newLocation += delta;
		}
		
		SetTranslation(newLocation.x, newLocation.y);
		bMoved = true;
	}

	if (bMoved) {
		UpdateOverlaps(pendingOverlaps, true);
	}

	const bool bAllowHitDispatch = !blockingHit.bStartPenetrating;
	if (blockingHit.bBlockingHit && bAllowHitDispatch) {
		if (bFilledHitResult) {
			DispatchBlockingHit(*actor, blockingHit);
		}
	}

	if (outHitResult) {
		if (bFilledHitResult) {
			*outHitResult = blockingHit;
		}
		else {
			*outHitResult = {};
		}
	}

	return bMoved;
}
