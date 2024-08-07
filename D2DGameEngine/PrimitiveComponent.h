#pragma once

#include "SceneComponent.h"

#include "CollisionShape.h"

#include "HitResult.h"

/**
 * @brief geometry to be rendered or used as collision.
 */
class PrimitiveComponent : public SceneComponent {
	using Super = SceneComponent;

public:
	bool isVisible{ true };
	bool bCanCollide{ false };
	bool bSimulatePhysics{ false };
	bool bApplyImpulseOnDamage{ false };

	// Physics
	float mass{ 1.0f };
	float frictionCoefficient{ 0.9f };
	float	dragForce{ 0.f };
	float minAcceleration{ 0.f };
	float maxAccelaration{ 10000.f };
	float minSpeed{ 0.f };
	float maxSpeed{ 1000.f };
	DXVec2 acceleration{};

	// Collision
	using OverlappingComponentSet = std::map<PrimitiveComponent*, HitResult>;
	OverlappingComponentSet previouslyOverlappingComponents;
	OverlappingComponentSet currentlyOverlappingComponents;

	// Damage
	bool bTakeDamage{ false };

public:
	PrimitiveComponent();
	~PrimitiveComponent();

	DXVec2 GetCenterOfMass() const {
		D2D_Point2F loc = GetComponentLocation();
		return { loc.x, loc.y };
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
	bool CheckComponentOverlapComponent(PrimitiveComponent* component, const DXVec2& pos) {
		return CheckComponentOverlapComponentImpl(component, pos);
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
		if (bPhysicalForce && bSimulatePhysics) {
			acceleration += force / mass;
		}
		else {
			acceleration += force;
		}
	}

	/**
	 * @brief 충격량을 더합니다.
	 * @param impuslse 충격량의 강도와 방향
	 * @param bPhysicalImpulse True 이면 속도의 변화에 질량이 적용됩니다.
	 */
	virtual void AddImpulse(DXVec2 impulse, bool bPhysicalImpulse) {
		if (bPhysicalImpulse) {
			velocity += impulse / mass;
		}
		else {
			velocity += impulse;
		}
	}


	// Damage System

	/**
	 * @brief 
	 * @param _damageAmount 
	 * @param damageEvent 
	 * @param instigator 
	 * @param damageCauser 
	 */
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

	// Collision
	virtual CollisionEnabled::Type GetCollisionEnabled() const override {
		return CollisionEnabled::NoCollision;
	}

	virtual ECollisionChannel GetCollisionObjectType() const override {
		return ECollisionChannel::WorldStatic;
	}

	virtual BoxCircleBounds CalculateBounds(const D2D_Mat3x2F& _worldTransform) override {
		return BoxCircleBounds{};
	}

	virtual void UpdateBounds() override {}

	virtual void UpdateOverlaps() override {
		// Check begin overlap
		for (auto& [otherCollider, collsionInfo] : currentlyOverlappingComponents) {
			auto it = previouslyOverlappingComponents.find(otherCollider);
			if (it == previouslyOverlappingComponents.end()) {
				//if (_notify) _notify->OnBeginOverlap(this, otherCollider, collsionInfo);
			}
		}
		// Check end overlap
		for (auto& [otherCollider, collsionInfo] : previouslyOverlappingComponents) {
			auto it = currentlyOverlappingComponents.find(otherCollider);
			if (it == currentlyOverlappingComponents.end()) {
				//if (_notify) _notify->OnEndOverlap(this, otherCollider, collsionInfo);
			}
		}
	}

	virtual DXVec2 GetComponentVelocity() const override {
		return Super::GetComponentVelocity();
	}

	bool IsSimulatingPhysics() override {
		return bSimulatePhysics;
	}

	// Physics update
	virtual void PreUpdate(float _dt) override {
	}

	virtual void Update(float _dt) override {
		if (bSimulatePhysics) {
			// Simple semi-implicit Euler integration
			acceleration -= velocity * dragForce; // Damping effect
			velocity += acceleration * _dt;
			acceleration = { 0, 0 }; // Reset acceleration each frame

			// Clamp velocities
			float speed = velocity.Length();
			DXVec2 unitVel = velocity;
			unitVel.Normalize();
			speed = speed < minSpeed ? minSpeed : maxSpeed < speed ? maxSpeed : speed;
			velocity = unitVel * speed;

			// Apply the movement
			MoveComponent(velocity * _dt, true, nullptr);
		}
		else {
			Super::Update(_dt);
		}
	}

	virtual void PostUpdate(float _dt) override {
	}

protected:

	virtual bool CheckComponentOverlapComponentImpl(
		PrimitiveComponent* component, 
		const DXVec2& pos) {
		return false;
	}

	virtual bool MoveComponentImpl(
		const D2D_Vec2F& delta,
		bool bSweep,
		HitResult& outHitResult) {
		// TODO:
		return false;
	}

};

// TODO: Line trace, sphere trace, sphere overlap