#pragma once

#include "SceneComponent.h"

#include "CollisionShape.h"

class PrimitiveComponent : public SceneComponent {
	bool isVisible{ true };
	bool isCollision{ false };
	bool isPhysicsCollision{ false };
	bool bApplyImpulseOnDamage{ false };

	// Physics
	float mass{ 1.0f };
	float	dragForce;
	float minAcceleration;
	float maxAccelaration;
	float minSpeed;
	float maxSpeed;
	DXVec2 accelaration;
	DXVec2 velocity;

	// Health
	bool bTakeDamage{ false };
	float health;

public:
	PrimitiveComponent();
	~PrimitiveComponent();

	DXVec2 GetCenterOfMass() const {
		// TODO:
	}

	virtual float GetMass() const { return mass; }

	virtual CollisionShape GetCollisionShape() const {
		// TODO:
		return CollisionShape();
	}

	/**
	 * @brief 컴포넌트가 특정 위치에서 다른 컴포넌트와 겹치는지 확인합니다.
	 * @param component 오버랩 테스트할 컴포넌트
	 * @param pos 이 컨포넌트의 위치
	 * @return 겹치면
	 */
	bool CheckComponentOverlapComponent(
		PrimitiveComponent* component,
		const DXVec2 pos) {
		// TODO:
	}

	virtual bool CheckLineTraceComponent(
		HitResult& outHit,
		const DXVec2 start,
		const DXVec2 end) {
		// TODO:
		return true;
	}

	void DispatchBlockingHit() {
		// TODO:
	}

	// Physics System

	/**
	 * @brief 힘을 더합니다. 
	 * @param force 힘의 강도와 방향
	 * @param bPhysicalForce True 이면 가속도의 변화량에 질량이 적용됩니다.
	 */
	virtual void AddForce(DXVec2 force, bool bPhysicalForce) {
		if (bPhysicalForce) {
			// TODO:
		}
		else {
			// TODO:
		}
	}

	/**
	 * @brief 충격량을 더합니다.
	 * @param impuslse 충격량의 강도와 방향
	 * @param bPhysicalImpulse True 이면 속도의 변화에 질량이 적용됩니다.
	 */
	virtual void AddImpulse(DXVec2 impuslse, bool bPhysicalImpulse) {
		if (bPhysicalImpulse) {
			// TODO:
		}
		else {
			// TODO:
		}
	}


	// Damage System
	
	virtual void ReceiveComponentDamage(
		float _damageAmount,
		const struct DamageType& damageEvent,
		class Controller* instigator,	// 데미지를 준 Player Controller 아니면 AIController
		class Actor* damageCauser	// 데미지를 준 오브젝트. 예시, 총알, 수류탄 등.
	) {
		// TODO:
	}

	// Collision Callbacks
	// TODO: Cursor Events
	virtual void OnBeginCursorOver() {}
	virtual void OnEndCursorOver() {}
	virtual void OnClicked() {}
	// Collision
	virtual void OnComponentBeginOverlap() {}
	virtual void OnComponentEndOverlap() {}
	virtual void OnComponentHit() {}

	// Tick
	virtual void Update(float _dt) override {
		// TODO:
	}

protected:

	virtual bool MoveComponentImpl(
		const D2D_Vec2F& delta,
		bool bSweep,
		HitResult& outHitResult) {
		// TODO:
		return false;
	}

};

// TODO: Line trace, sphere trace, sphere overlap