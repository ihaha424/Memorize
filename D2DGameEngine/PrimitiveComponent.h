#pragma once

#include "SceneComponent.h"

#include "CollisionShape.h"

class PrimitiveComponent : public SceneComponent {
	bool isVisible{ true };
	bool isCollision{ false };
	bool isPhysicsCollision{ false };

public:
	PrimitiveComponent();
	~PrimitiveComponent();

	DXVec2 GetCenterOfMass() const {

	}

	virtual DXVec2 GetMass() const {}

	virtual CollisionShape GetCollisionShape() const {

	}

	// TODO: Cursor Events
	virtual void OnBeginCursorOver();
	virtual void OnEndCursorOver();
	virtual void OnClicked();
	// Collision
	virtual void OnComponentBeginOverlap();
	virtual void OnComponentEndOverlap();
	virtual void OnComponentHit();

	// Physics System



	// Damage System
	
	virtual void ReceiveComponentDamage(
		float _damageAmount,
		const struct DamageType& damageEvent,
		class Controller* instigator,	// 데미지를 준 Player Controller 아니면 AIController
		class Actor* damageCauser	// 데미지를 준 오브젝트. 예시, 총알, 수류탄 등.
	);

};

// TODO: Line trace, sphere trace, sphere overlap