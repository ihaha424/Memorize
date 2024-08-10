#include "BoxComponent.h"

#include "D2DRenderer.h"

#include "IntersectionUtil.h"


bool BoxComponent::CheckSweepComponent(
	HitResult& outHit, 
	const DXVec2& start, 
	const DXVec2& end, 
	const DXMat4x4& rotation, 
	const CollisionShape& collisionShape, 
	const ECollisionChannel collisionChannel,
	const CollisionProperty& collisionProperty
) {
	outHit = {};
	// Early out if collision is not enabled
	if (!bCanCollide) return false;

	if (this->collisionProperty.GetCollisionResponse(collisionChannel) == CollisionResponse::Ignore ||
		collisionProperty.GetCollisionResponse(GetCollisionObjectType()) == CollisionResponse::Ignore) return false;

	CollisionShape myCollisionShape;
	if (!this->GetCollisionShape(1.f, myCollisionShape)) return false;

	if (myCollisionShape.IsNearlyZero() || collisionShape.IsNearlyZero())
		return false;

	// Build my Box
	Box myBox = Box::BuildAABB({ 0.f, 0.f }, boxExtent);
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
	bool bStartPenetrating{ true };
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

		hasHit = res;
		if (hasHit) break;

		pos += deltaDir * stepSize;
		bStartPenetrating = false;
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
		outHit.bStartPenetrating = bStartPenetrating;
		outHit.hitComponent = this;

		outHit.location = pos;
		outHit.distance = (pos - start).Length();

		outHit.traceStart = start;
		outHit.traceEnd = end;

		outHit.time = outHit.distance / deltaSize;
	}

	return hasHit;
}

bool BoxComponent::CheckOverlapComponent(
	OverlapResult& outOverlap, 
	const DXVec2& pos, 
	const DXMat4x4& rotation, 
	const CollisionShape& collisionShape, 
	const ECollisionChannel collisionChannel, 
	const CollisionProperty& collisionProperty
) {
	if (!bCanCollide) return false;

	if (this->collisionProperty.GetCollisionResponse(collisionChannel) == CollisionResponse::Ignore) 
		return false;

	CollisionShape myCollisionShape;
	if (!this->GetCollisionShape(1.f, myCollisionShape)) 
		return false;

	if (myCollisionShape.IsNearlyZero() || collisionShape.IsNearlyZero())
		return false;

	// Build my Box
	Box myBox = Box::BuildAABB({ 0.f, 0.f }, boxExtent);
	myBox.ul = DXVec2::Transform(myBox.ul, GetWorldTransform());
	myBox.lr = DXVec2::Transform(myBox.lr, GetWorldTransform());

	bool res{ false };
	HitResult hitResult;
	switch (collisionShape.shapeType)
	{
	case ECollisionShape::Box: {
		// Build box from the collision shape and rotation
		Box otherBox = Box::BuildAABB(pos, collisionShape.GetExtent());
		otherBox.ul = DXVec2::Transform(otherBox.ul, rotation);
		otherBox.lr = DXVec2::Transform(otherBox.lr, rotation);
		res = intersectionUtil::BoxBoxIntersectWithResult(otherBox, myBox, hitResult);
	}	break;
	case ECollisionShape::Capsule: {
		// Build Capsule from the collision shape and rotation
		Capsule otherCapsule{
			.center = pos,
			.direction = DXVec2::Transform(DXVec2::UnitY, rotation),
			.extent = collisionShape.GetCapsuleHalfHeight(),
			.radius = collisionShape.GetCapsuleRadius()
		};
		res = intersectionUtil::CapsuleBoxIntersectWithResult(otherCapsule, myBox, hitResult);
	}	break;
	case ECollisionShape::Circle: {
		// Build Circle from the collision shape and rotation
		Circle otherCircle{
			.center = pos,
			.radius = collisionShape.GetCircleRadius()
		};
		res = intersectionUtil::CircleBoxIntersectWithResult(otherCircle, myBox, hitResult);
	}	break;
	case ECollisionShape::Polygon: {
		// Build Polygon from the collision shape and rotation
		TPolygon otherPolygon{ std::move(collisionShape.GetPolygonVertices()) };
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
		if (collisionProperty.GetCollisionResponse(this->GetCollisionObjectType()) == CollisionResponse::Block &&
			this->collisionProperty.GetCollisionResponse(collisionProperty.objectType) == CollisionResponse::Block)
		{
			bBlockingHit = true;
		}

		outOverlap.bBlockingHit = bBlockingHit;
		outOverlap.component = this;
	}

	return res;
}

void BoxComponent::Render(D2DRenderer* _renderer)
{
	_renderer->PushTransform(GetWorldTransform());

#ifndef NDEBUG
	D2D_RectF dest{
		.left = -boxExtent.width / 2.f,
		.top = boxExtent.height / 2.f,
		.right = boxExtent.width / 2.f,
		.bottom = -boxExtent.height / 2.f
	};

	_renderer->DrawBorder(
		{ dest.left, dest.top },
		{ dest.right, dest.bottom },
		D2D_Color::Red
	);
#endif

	_renderer->PopTransform();
}

bool BoxComponent::CheckComponentOverlapComponentImpl(
	PrimitiveComponent* primComp, 
	const DXVec2& pos, 
	const DXMat4x4& rotation
) {
	if (!bCanCollide || !primComp->bCanCollide) return false;
	if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Ignore)
		return false;

	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	OverlapResult overlapResult;
	bool res = CheckOverlapComponent(
		overlapResult,
		pos, rotation,
		otherCollisionShape,
		primComp->GetCollisionObjectType(),
		primComp->collisionProperty
	);

	return res;
}

bool BoxComponent::CheckComponentOverlapComponentWithResultImpl(
	PrimitiveComponent* primComp, 
	const DXVec2& pos, 
	const DXMat4x4& rotation, 
	std::vector<OverlapResult>& outOverlap
) {
	if (!bCanCollide || !primComp->bCanCollide) return false;
	if (collisionProperty.GetCollisionResponse(primComp->GetCollisionObjectType()) == CollisionResponse::Ignore) 
		return false;
	
	CollisionShape otherCollisionShape;
	primComp->GetCollisionShape(1.f, otherCollisionShape);

	OverlapResult overlapResult;
	bool res = CheckOverlapComponent(
		overlapResult,
		pos, rotation,
		otherCollisionShape,
		primComp->GetCollisionObjectType(),
		primComp->collisionProperty
	);

	if (res) {
		outOverlap.push_back(overlapResult);
	}

	return res;
}
