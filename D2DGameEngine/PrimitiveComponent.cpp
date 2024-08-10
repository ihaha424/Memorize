#include "PrimitiveComponent.h"

#include "World.h"
#include "IntersectionUtil.h"

#include "DamageEvent.h"

PrimitiveComponent::PrimitiveComponent(Actor* _owner) : SceneComponent(_owner) {}

PrimitiveComponent::~PrimitiveComponent() {}

void PrimitiveComponent::ReceiveComponentDamage(float _damageAmount, const DamageEvent& damageEvent, Controller* instigator, Actor* damageCauser)
{
	if (bApplyImpulseOnDamage) 
	{	
		if (damageEvent.damageEventType == DamageEventType::PointDamage) 
		{
			const PointDamageEvent& pointDamageEvent = static_cast<const PointDamageEvent&>(damageEvent);
			if (pointDamageEvent.damageType.damageImpulse > 0.f && !(abs(pointDamageEvent.shotDirection.Length()) < EPSILON)) 
			{
				Math::Vector2 impulseToApply = pointDamageEvent.shotDirection * pointDamageEvent.damageType.damageImpulse;

				AddImpulse(impulseToApply, IsSimulatingPhysics());
			}
		} 
		else if (damageEvent.damageEventType == DamageEventType::RadialDamage) 
		{
			const RadialDamageEvent& radialDamageEvent = static_cast<const RadialDamageEvent&>(damageEvent);
			if (radialDamageEvent.damageType.damageImpulse > 0.f)
			{
				Math::Vector2 impulseDirection = GetComponentLocation() - radialDamageEvent.origin; impulseDirection.Normalize();
				Math::Vector2 impulseToApply = impulseDirection * radialDamageEvent.damageType.damageImpulse;

				AddImpulse(impulseToApply, IsSimulatingPhysics());
			}
		}
	}
}

void PrimitiveComponent::SetCollisionEnabled(CollisionEnabled::Type type)
{
	collisionProperty.collisionEnabled = type;
	if (type == CollisionEnabled::NoCollision) {
		bCanCollide = false;
		// Unregister collision
		GetWorld()->UnregisterComponentCollision(this);
	}
	else {
		bCanCollide = true;
		// Register collision
		GetWorld()->RegisterComponentCollision(this);
	}
}

bool PrimitiveComponent::IsCollisionRegistered()
{
	GetWorld()->IsComponentCollisionRegistered(this);
	return false;
}

bool PrimitiveComponent::CheckSweepComponent(
	HitResult& outHit, 
	const DXVec2& start, 
	const DXVec2& end, 
	const DXMat4x4& rotation, 
	const CollisionShape& collisionShape, 
	const ECollisionChannel collisionChannel,
	const CollisionProperty& collisionProperty)
{
	outHit = {};
	// Early out if collision is not enabled
	if (!bCanCollide) return false;

	if (GetCollisionObjectType() != collisionChannel) return false;

	CollisionShape myCollisionShape;
	this->GetCollisionShape(1.f, myCollisionShape);

	if (myCollisionShape.IsNearlyZero() || collisionShape.IsNearlyZero())
		return false;

	if (myCollisionShape.shapeType == ECollisionShape::Box) {
		// Build my Box
		Box myBox = Box::BuildAABB({ 0.f, 0.f }, myCollisionShape.GetExtent());
		myBox.ul = DXVec2::Transform(myBox.ul, GetWorldTransform());
		myBox.lr = DXVec2::Transform(myBox.lr, GetWorldTransform());

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
				res = intersectionUtil::BoxBoxIntersectWithResult(otherBox, myBox, outHit);
			}	break;
			case ECollisionShape::Capsule: {
				// Build Capsule from the collision shape and rotation
				Capsule otherCapsule{
					.center = pos,
					.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
					.extent = collisionShape.GetCapsuleHalfHeight(),
					.radius = collisionShape.GetCapsuleRadius()
				};
				res = intersectionUtil::CapsuleBoxIntersectWithResult(otherCapsule, myBox, outHit);
			}	break;
			case ECollisionShape::Circle: {
				// Build Circle from the collision shape and rotation
				Circle otherCircle{
					.center = pos,
					.radius = collisionShape.GetCircleRadius()
				};
				res = intersectionUtil::CircleBoxIntersectWithResult(otherCircle, myBox, outHit);
			}	break;
			case ECollisionShape::Polygon: {
				// Build Polygon from the collision shape and rotation
				TPolygon otherPolygon{ std::move(collisionShape.GetPolygonVertices()) };
				for (Math::Vector2& point : otherPolygon.points) {
					point += pos;
					point = DXVec2::Transform(point, rotation);
				}
				res = intersectionUtil::PolygonBoxIntersectWithResult(otherPolygon, myBox, outHit);
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
			// outHit.bStartPenetrating = true;	// TODO: Can't check here.
			outHit.hitComponent = this;

			outHit.location = pos;
			outHit.distance = (pos - start).Length();

			outHit.traceStart = start;
			outHit.traceEnd = end;

			outHit.time = outHit.distance / deltaSize;
		}

		return hasHit;
	}
	else if (myCollisionShape.shapeType == ECollisionShape::Capsule) {
		// Build my Capsule
		Math::Matrix world = GetWorldTransform();
		Math::Matrix rotation = Math::ExtractRotation(world);
		Capsule myCapsule{
			.center = GetComponentLocation(),
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = myCollisionShape.GetCapsuleHalfHeight(),
			.radius = myCollisionShape.GetCapsuleRadius()
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
	else if (myCollisionShape.shapeType == ECollisionShape::Circle) {
		// Build my Circle
		Circle myCircle{
			.center = GetComponentLocation(),
			.radius = myCollisionShape.GetCircleRadius()
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
				res = intersectionUtil::BoxCircleIntersectWithResult(otherBox, myCircle, outHit);
			}	break;
			case ECollisionShape::Capsule: {
				// Build Capsule from the collision shape and rotation
				Capsule otherCapsule{
					.center = pos,
					.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
					.extent = collisionShape.GetCapsuleHalfHeight(),
					.radius = collisionShape.GetCapsuleRadius()
				};
				res = intersectionUtil::CapsuleCircleIntersectWithResult(otherCapsule, myCircle, outHit);
			}	break;
			case ECollisionShape::Circle: {
				// Build Circle from the collision shape and rotation
				Circle otherCircle{
					.center = pos,
					.radius = collisionShape.GetCircleRadius()
				};
				res = intersectionUtil::CircleCircleIntersectWithResult(otherCircle, myCircle, outHit);
			}	break;
			case ECollisionShape::Polygon: {
				// Build Polygon from the collision shape and rotation
				TPolygon otherPolygon{ std::move(collisionShape.GetPolygonVertices()) };
				for (Math::Vector2& point : otherPolygon.points) {
					point += pos;
					point = DXVec2::Transform(point, rotation);
				}
				res = intersectionUtil::PolygonCircleIntersectWithResult(otherPolygon, myCircle, outHit);
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
	else if (myCollisionShape.shapeType == ECollisionShape::Polygon) {
		// Build my Polygon
		TPolygon myPolygon{ myCollisionShape.GetPolygonVertices() };
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

	return false;
}

bool PrimitiveComponent::CheckOverlapComponent(
	OverlapResult& outHit, 
	const DXVec2& pos, 
	const DXMat4x4& rotation, 
	const CollisionShape& collisionShape, 
	const ECollisionChannel collisionChannel, 
	const CollisionProperty& collisionProperty
) {
	return false;
}

void PrimitiveComponent::DispatchBlockingHit(Actor& owner, HitResult& blockingHit)
{
	PrimitiveComponent* blockingHitComponent = blockingHit.hitComponent;
	if (blockingHitComponent) {
		owner.NotifyBlockingHit(this, blockingHitComponent, true, blockingHit);

		Actor* blockingHitActor = blockingHitComponent->GetOwner();
		if (blockingHitActor) {
			blockingHitActor->NotifyBlockingHit(blockingHitComponent, this, false, blockingHit);
		}
	}
}

void PrimitiveComponent::BeginComponentOverlap(const OverlapInfo& otherOverlap, bool bDoNotifies)
{
	PrimitiveComponent* otherComp = otherOverlap.overlapInfo.hitComponent;

	Actor* otherActor = otherComp->GetOwner();
	Actor* myActor = GetOwner();

	if (myActor == otherActor) return;

	PushOverlappingComponent(otherComp, otherOverlap);

	if (bDoNotifies) 
	{
		OnComponentBeginOverlap();
		otherComp->OnComponentBeginOverlap();

		myActor->NotifyActorBeginOverlap(otherActor);
		otherActor->NotifyActorBeginOverlap(myActor);
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

		myActor->NotifyActorEndOverlap(otherActor);
		otherActor->NotifyActorEndOverlap(myActor);
	}
}

void PrimitiveComponent::UpdateOverlaps(
	const std::vector<OverlapInfo>* newOverlaps, 
	bool bDoNotifies
) {

	if (bGenerateOverlapEvent && IsCollisionEnabled()) {

		if (newOverlaps) {
			for (const OverlapInfo& overlapInfo : *newOverlaps) {
				BeginComponentOverlap(overlapInfo, bDoNotifies);
			}
		}

		OverlappingComponentSet newOverlappingComponents;
		if (bGenerateOverlapEvent) {
			World* myWorld = GetWorld();
			std::vector<OverlapResult> overlaps;
			myWorld->CheckComponentOverlapMulti(overlaps, this, GetComponentLocation(), R);

			for (OverlapResult& overlapResult : overlaps)
			{
				PrimitiveComponent* hitComp = overlapResult.component;
				if (hitComp && (hitComp != this) && hitComp->bGenerateOverlapEvent)
				{
					newOverlappingComponents.insert({
						hitComp,
						OverlapInfo{
							false,
							HitResult{
								.bBlockingHit = overlapResult.bBlockingHit,
								.hitComponent = hitComp
							}
						}
					});
				}
			}
		}

		OverlappingComponentSet oldOverlappingComponents{ std::move(currentlyOverlappingComponents) };
		currentlyOverlappingComponents.clear();

		// Check begin overlap
		for (auto& [otherComponent, overlapInfo] : newOverlappingComponents) {
			if (this == otherComponent) continue;
			auto it = oldOverlappingComponents.find(otherComponent);
			if (it == oldOverlappingComponents.end()) {
				BeginComponentOverlap(overlapInfo, bDoNotifies);
			}
		}

		// Check end overlap
		for (auto& [otherComponent, overlapInfo] : oldOverlappingComponents) {
			auto it = newOverlappingComponents.find(otherComponent);
			if (it == newOverlappingComponents.end()) {
				EndComponentOverlap(overlapInfo, bDoNotifies);
			}
			else {
				PushOverlappingComponent(otherComponent, overlapInfo);
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
		// Build my Capsule
		Math::Matrix world = GetWorldTransform();
		Math::Matrix rotation = Math::ExtractRotation(world);
		Capsule myCapsule{
			.center = GetComponentLocation(),
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = myCollisionShape.GetCapsuleHalfHeight(),
			.radius = myCollisionShape.GetCapsuleRadius()
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
	else if (myCollisionShape.shapeType == ECollisionShape::Circle) {	
		// Build my Circle
		Circle myCircle{
			.center = GetComponentLocation(),
			.radius = myCollisionShape.GetCircleRadius()
		};

		bool res{ false };
		switch (otherCollisionShape.shapeType)
		{
		case ECollisionShape::Box: {
			// Build box from the collision shape and rotation
			Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
			otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
			otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
			res = intersectionUtil::BoxCircleIntersect(otherBox, myCircle);
		}	break;
		case ECollisionShape::Capsule: {
			// Build Capsule from the collision shape and rotation
			Capsule otherCapsule{
				.center = pos,
				.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
				.extent = otherCollisionShape.GetCapsuleHalfHeight(),
				.radius = otherCollisionShape.GetCapsuleRadius()
			};
			res = intersectionUtil::CircleCapsuleIntersect(myCircle, otherCapsule);
		}	break;
		case ECollisionShape::Circle: {
			// Build Circle from the collision shape and rotation
			Circle otherCircle{
				.center = pos,
				.radius = otherCollisionShape.GetCircleRadius()
			};
			res = intersectionUtil::CircleCircleIntersect(myCircle, otherCircle);
		}	break;
		case ECollisionShape::Polygon: {
			// Build Polygon from the collision shape and rotation
			TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
			for (Math::Vector2& point : otherPolygon.points) {
				point += pos;
				point = DXVec2::Transform(point, rotation);
			}
			res = intersectionUtil::CirclePolygonIntersect(myCircle, otherPolygon);
		}	break;
		}

		return res;
	}
	else if (myCollisionShape.shapeType == ECollisionShape::Polygon) {
		// Build my Polygon
		TPolygon myPolygon{ myCollisionShape.GetPolygonVertices() };
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

	return false;
}

bool PrimitiveComponent::CheckComponentOverlapComponentWithResultImpl(
	PrimitiveComponent* primComp,
	const DXVec2& pos, const DXMat4x4& rotation,
	std::vector<OverlapResult>& outOverlap) {	// TODO:

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
		HitResult hitResult;
		switch (otherCollisionShape.shapeType)
		{
		case ECollisionShape::Box: {
			// Build box from the collision shape and rotation
			Box otherBox = Box::BuildAABB(pos, otherCollisionShape.GetExtent());
			otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
			otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
			res = intersectionUtil::BoxBoxIntersectWithResult(otherBox, myBox, hitResult);
		}	break;
		case ECollisionShape::Capsule: {
			// Build Capsule from the collision shape and rotation
			Capsule otherCapsule{
				.center = pos,
				.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
				.extent = otherCollisionShape.GetCapsuleHalfHeight(),
				.radius = otherCollisionShape.GetCapsuleRadius()
			};
			res = intersectionUtil::CapsuleBoxIntersectWithResult(otherCapsule, myBox, hitResult);
		}	break;
		case ECollisionShape::Circle: {
			// Build Circle from the collision shape and rotation
			Circle otherCircle{
				.center = pos,
				.radius = otherCollisionShape.GetCircleRadius()
			};
			res = intersectionUtil::CircleBoxIntersectWithResult(otherCircle, myBox, hitResult);
		}	break;
		case ECollisionShape::Polygon: {
			// Build Polygon from the collision shape and rotation
			TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
			for (Math::Vector2& point : otherPolygon.points) {
				point += pos;
				point = DXVec2::Transform(point, rotation);
			}
			res = intersectionUtil::PolygonBoxIntersectWithResult(otherPolygon, myBox, hitResult);
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
	else if (myCollisionShape.shapeType == ECollisionShape::Capsule) {
		// Build my Capsule
		Math::Matrix world = GetWorldTransform();
		Math::Matrix rotation = Math::ExtractRotation(world);
		Capsule myCapsule{
			.center = GetComponentLocation(),
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = myCollisionShape.GetCapsuleHalfHeight(),
			.radius = myCollisionShape.GetCapsuleRadius()
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
	else if (myCollisionShape.shapeType == ECollisionShape::Circle) {
		// Build my Circle
		Circle myCircle{
			.center = GetComponentLocation(),
			.radius = myCollisionShape.GetCircleRadius()
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
			res = intersectionUtil::BoxCircleIntersectWithResult(otherBox, myCircle, hitResult);
		}	break;
		case ECollisionShape::Capsule: {
			// Build Capsule from the collision shape and rotation
			Capsule otherCapsule{
				.center = pos,
				.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
				.extent = otherCollisionShape.GetCapsuleHalfHeight(),
				.radius = otherCollisionShape.GetCapsuleRadius()
			};
			res = intersectionUtil::CapsuleCircleIntersectWithResult(otherCapsule, myCircle, hitResult);
		}	break;
		case ECollisionShape::Circle: {
			// Build Circle from the collision shape and rotation
			Circle otherCircle{
				.center = pos,
				.radius = otherCollisionShape.GetCircleRadius()
			};
			res = intersectionUtil::CircleCircleIntersectWithResult(otherCircle, myCircle, hitResult);
		}	break;
		case ECollisionShape::Polygon: {
			// Build Polygon from the collision shape and rotation
			TPolygon otherPolygon{ std::move(otherCollisionShape.GetPolygonVertices()) };
			for (Math::Vector2& point : otherPolygon.points) {
				point += pos;
				point = DXVec2::Transform(point, rotation);
			}
			res = intersectionUtil::PolygonCircleIntersectWithResult(otherPolygon, myCircle, hitResult);
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
	else if (myCollisionShape.shapeType == ECollisionShape::Polygon) {
		// Build my Polygon
		TPolygon myPolygon{ myCollisionShape.GetPolygonVertices() };
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

	return false;

}

bool PrimitiveComponent::MoveComponentImpl(
	const DXVec2& delta, 
	const float angleDelta,
	bool bSweep, 
	HitResult* outHitResult)
{
	const Math::Matrix initialRotation = R;
	const Math::Matrix newRotation = R * Math::Matrix::CreateRotationZ(angleDelta);
	const Math::Vector2 traceStart = GetComponentLocation();
	const Math::Vector2 traceEnd = traceStart + delta;
	float deltaSizeSquared = delta.LengthSquared();

	// 만약 움직이는 정도가 너무 적으면 리턴합니다.
	const float minMovementDistSq = (bSweep ? (4.f * EPSILON) * (4.f * EPSILON) : 0.f);
	if (deltaSizeSquared <= minMovementDistSq)
	{
		// no rotation at all
		if (newRotation == initialRotation) {
			if (outHitResult)
			{
				outHitResult->traceStart = traceStart;
				outHitResult->traceEnd = traceEnd;
			}
			return true;
		}
		deltaSizeSquared = 0.f;
	}

	const bool bSkipPhysicsMove = !bSimulatePhysics;

	HitResult blockingHit{};
	blockingHit.bBlockingHit = false;
	blockingHit.time = 1.f;
	bool bFilledHitResult = false;
	bool bMoved = false;
	bool bRotationOnly = false;

	std::vector<OverlapInfo> pendingOverlaps;

	Actor* actor = GetOwner();

	if (!bSweep) 
	{	// No sweeping
		SetTranslation(traceEnd.x, traceEnd.y);
		Rotate(angleDelta);
		bMoved = true;
		bRotationOnly = (deltaSizeSquared == 0);
	}
	else 
	{	// Sweeping
		std::vector<HitResult> hits;
		DXVec2 newLocation = traceStart;

		const bool bCollisionEnabled = IsCollisionEnabled();
		World* myWorld = GetWorld();
		if (myWorld && bCollisionEnabled) 
		{
			if (actor) {
				// TODO: Debug Message
			}
			else {
				// TODO: Debug Message
			}

			// Sweep check
			Math::Matrix rotation = Math::ExtractRotation(GetWorldTransform());
			const bool bHadBlockingHit = myWorld->CheckComponentSweepMulti(hits, this, traceStart, traceEnd, rotation);

			// Pullback hit time info
			if (hits.size() > 0) {
				const float deltaSize = delta.Length();
				for (uint i = 0; i < hits.size(); ++i) {
					PullBackHit(hits[i], traceStart, traceEnd, deltaSize);
				}
			}

			// Collision Event check.
			int firstNonInitialOverlapIndex = -1;
			if (bHadBlockingHit || bGenerateOverlapEvent) 
			{
				int blockingHitIndex = -1;	// Find the first blocking hit.
				float blockingHitNormalDotDelta = (std::numeric_limits<float>::max)();

				// Query overlapping infomation
				for (int hitIndex = 0; hitIndex < hits.size(); hitIndex++) 
				{
					const HitResult& testHit = hits[hitIndex];

					// 1. Blocking Hit!
					if (testHit.bBlockingHit) 
					{
						// Prevent overlapping with components in the same actor.
						if (testHit.hitComponent->GetOwner() != GetOwner()) 
						{
							if (testHit.bStartPenetrating) 
							{
								// There could be multiple initial hits.
								const float normalDotDelta = testHit.impactNormal.Dot(delta);
								if (normalDotDelta < blockingHitNormalDotDelta) 
								{
									blockingHitNormalDotDelta = normalDotDelta;
									blockingHitIndex = hitIndex;
								}
							}
							else if (blockingHitIndex == -1)
							{
								// First non-overlapping blocking hit detected.
								// Break out from the loop and stop querying overlap info.
								blockingHitIndex = hitIndex;
								break;
							}
						}
					}
					// 2. Overlapping!
					else if (bGenerateOverlapEvent) 
					{
						PrimitiveComponent* overlapComponent = testHit.hitComponent;

						if (overlapComponent && overlapComponent->bGenerateOverlapEvent)
						{
							// Prevent overlapping with components in the same actor.
							if (overlapComponent->GetOwner() != GetOwner())
							{
								// Don't process overlap events happens after blocking hit.
								if (blockingHitIndex >= 0 && // Blocking hit exists
									testHit.time > hits[blockingHitIndex].time	// Occurs after blocking hit
								) {
									// Exit the loop.
									break;
								}

								// First overlap.
								if (firstNonInitialOverlapIndex == -1 && testHit.time > 0.f) {
									firstNonInitialOverlapIndex = pendingOverlaps.size();
								}

								// Add an overlap event.
								pendingOverlaps.push_back(
									OverlapInfo{ 
										.bFromSweep = bSweep, 
										.overlapInfo = testHit 
									}
								);
							}
						}
					}
				}	// END for (int hitIndex = 0; hitIndex < hits.size(); hitIndex++) 

				// Update blocking hit if there is a one.
				if (blockingHitIndex >= 0) {
					blockingHit = hits[blockingHitIndex];
					bFilledHitResult = true;
				}
			}	// END if (bHadBlockingHit || bGenerateOverlapEvent) 

			if (!blockingHit.bBlockingHit) 
			{
				newLocation = traceEnd;
			}
			else 
			{
				// Check if bFilledHitResult == true
				newLocation = traceStart + (blockingHit.time * (traceEnd - traceStart));
			
				// Check if the new location is ignorable
				const DXVec2 toNewLocation = (newLocation - traceStart);
				if (toNewLocation.LengthSquared() <= minMovementDistSq)
				{
					newLocation = traceStart;
					blockingHit.time = 0.f;

					pendingOverlaps.resize(firstNonInitialOverlapIndex+1);
				}
			}	// END if (!blockingHit.bBlockingHit)

		}
		else if (deltaSizeSquared > 0.f) {
			newLocation += delta;
		}
		else if (deltaSizeSquared == 0.f && bCollisionEnabled) {
			bRotationOnly = true;
		}	// END if (myWorld && bCollisionEnabled) 
		
		// Move the component
		SetTranslation(newLocation.x, newLocation.y);
		bMoved = true;
	}	// END if (!bSweep) 

	if (bMoved) {
		UpdateOverlaps(&pendingOverlaps, true);
	}

	if (blockingHit.bBlockingHit && !blockingHit.bStartPenetrating) {
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
