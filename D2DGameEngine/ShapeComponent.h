#pragma once

#include "PrimitiveComponent.h"

class ShapeComponent : public PrimitiveComponent {

public:
	ShapeComponent(Actor* _owner)  : PrimitiveComponent(_owner){
#ifndef NDEBUG
		isVisible = true;
#else 
		isVisible = false;
#endif
		SetCollisionEnabled(CollisionEnabled::EnableCollision);
		SetTickProperties(TICK_PHYSICS | TICK_RENDER);
	}

	// Collision
	virtual bool IsCollisionEnabled() const {
		return collisionProperty.collisionEnabled == CollisionEnabled::EnableCollision;
	}

	virtual CollisionEnabled::Type GetCollisionEnabled() const override {
		return collisionProperty.collisionEnabled;
	}

	virtual ECollisionChannel GetCollisionObjectType() const override {
		return collisionProperty.objectType;
	}
};
