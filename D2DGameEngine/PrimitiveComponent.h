#pragma once

#include "SceneComponent.h"

#include "CollisionShape.h"
#include "CollisionProperty.h"

#include "HitResult.h"
#include "OverlapResult.h"
#include "OverlapInfo.h"

#include "Debug.h"

class Actor;
/**
 * @brief geometry to be rendered or used as collision.
 */
class PrimitiveComponent : public SceneComponent {
	using Super = SceneComponent;

public:
	bool isVisible{ true };			// false 면 렌더 안함
	bool bRenderDirty{ false };	// Y 값이 변하면 렌더 더티

	// A hit event generated only for one of the components 
	// that has bSimulationPhysics true.
	// It is because sweeping can be done only when physics is on.
	bool bSimulatePhysics{ false };
	bool bApplyImpulseOnDamage{ false };	// TODO: damage system

	// Physics
	float mass{ 1.0f };
	float frictionCoefficient{ 0.2f };
	float	dragForce{ 0.f };
	float minAcceleration{ -1000.f };
	float maxAccelaration{ 10000.f };
	float minSpeed{ 0.f };
	float maxSpeed{ 500.f };
	DXVec2 acceleration{};

	// Collision
	bool bCanCollide{ false };
	bool bGenerateOverlapEvent{ false };
	CollisionProperty collisionProperty;	// Default NoCollision
	using OverlappingComponentSet = std::unordered_map<PrimitiveComponent*, OverlapInfo>;
	OverlappingComponentSet currentlyOverlappingComponents;

	// Damage
	bool bTakeDamage{ false };

public:
	PrimitiveComponent(Actor* _owner);
	~PrimitiveComponent();

	DXVec2 GetCenterOfMass() const {
		return GetComponentLocation();
	}

	virtual float GetMass() const { return mass; }

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
		const struct DamageEvent& damageEvent,
		class Controller* instigator,	// 데미지를 준 Player Controller 아니면 AIController
		class Actor* damageCauser	// 데미지를 준 오브젝트. 예시, 총알, 수류탄 등.
	);

	// Collision Callbacks
	// Collision
	virtual void OnComponentBeginOverlap() {}
	virtual void OnComponentOverlap() {}
	virtual void OnComponentEndOverlap() {}

	void SetCollisionEnabled(CollisionEnabled::Type type);

	bool IsCollisionRegistered();

	void SetCollisionObjectType(ECollisionChannel objectType) {
		collisionProperty.objectType = objectType;
	}

	void SetCollisionResponseToChannels(CollisionResponseContainer responseContainer) {
		collisionProperty.responseContainer = responseContainer;
	}

	// Collision
	virtual bool IsCollisionEnabled() const {
		return collisionProperty.collisionEnabled == CollisionEnabled::EnableCollision;
	}

	virtual CollisionEnabled::Type GetCollisionEnabled() const override {
		return CollisionEnabled::NoCollision;
	}

	virtual ECollisionChannel GetCollisionObjectType() const override {
		return ECollisionChannel::WorldStatic;
	}

	/**
	 * @brief 콜리션 도형을 리턴합니다. 콜리션 도형은 컴포넌트의 스케일 값을 반영해야 합니다.
	 * @param Inflation 
	 * @param CollisionShape 
	 * @return 
	 */
	virtual bool GetCollisionShape(float Inflation, CollisionShape& CollisionShape) const {
		return false;
	}

	virtual bool IsZeroExtent() const {
		return true;
	}

	/**
	 * @brief 다른 컴포넌트가 특정 위치에서 컴포넌트와 겹치는지 확인합니다.
	 * @param component 오버랩 테스트할 컴포넌트
	 * @param pos 다른 컨포넌트를 테스트할 위치
	 * @param rotation 다른 컴포넌트의 로테이션 매트릭스
	 * @return True 이면 오버랩 합니다.
	 */
	bool CheckComponentOverlapComponent(PrimitiveComponent* component, const DXVec2& pos, const DXMat4x4& rotation) {
		return CheckComponentOverlapComponentImpl(component, pos, rotation);
	}

	bool CheckComponentOverlapComponentWithResult(
		PrimitiveComponent* component, const DXVec2& pos, const DXMat4x4& rotation,
		std::vector<OverlapResult>& outOverlap) {
		return CheckComponentOverlapComponentWithResultImpl(component, pos, rotation, outOverlap);
	}

	virtual bool CheckLineTraceComponent(
		HitResult& outHit,
		const DXVec2 start,
		const DXVec2 end,
		float width) {
		// TODO:
		return true;
	}

	virtual bool CheckSweepComponent(
		HitResult& outHit,
		const DXVec2& start,
		const DXVec2& end,
		const DXMat4x4& rotation,
		const CollisionShape& collisionShape,
		const ECollisionChannel collisionChannel,
		const CollisionProperty& collisionProperty
	);

	virtual bool CheckOverlapComponent(
		OverlapResult& outOverlap,
		const DXVec2& pos,
		const DXMat4x4& rotation,
		const CollisionShape& collisionShape,
		const ECollisionChannel collisionChannel,
		const CollisionProperty& collisionProperty
	);

	/**
	 * @brief Dispatch a blocking hit event to the owner.
	 * @param owner 
	 * @param blockingHit 
	 */
	void DispatchBlockingHit(Actor& owner, HitResult& blockingHit);

	/**
	 * @brief Checks if another component is currently overlapping with this component
	 * @param otherComp 
	 * @return True when overlapping. False, otherwise.
	 */
	bool IsOverlappingComponent(PrimitiveComponent* otherComp) const {
		auto it = currentlyOverlappingComponents.find(otherComp);
		return it != currentlyOverlappingComponents.end();
	}

	/**
	 * @brief Begin tracking an overlap interaction with the component specified.
	 * @param otherOverlap 
	 * @param bDoNotifies 
	 */
	void BeginComponentOverlap(const OverlapInfo& overlap, bool bDoNotifies);

	/**
	 * @brief Receive begin component overlap event and do all the converting stuff.
	 */
	void ReceiveBeginComponentOverlap(PrimitiveComponent* otherComp, const bool bFromSweep, const HitResult& overlapInfo);

	/**/
	void ComponentOverlap(const OverlapInfo& overlap, bool bDoNotifies);

	/**
	 * @brief End tracking an overlap interaction
	 * @param otherOverlap 
	 * @param bDoNotifies 
	 * @param bSkipNotifySelf 
	 */
	void EndComponentOverlap(const OverlapInfo& overlap, bool bDoNotifies, bool bSkipNotifySelf=false);

	/**
	 * @brief Receive end component overlap event and do all the converting stuff.
	 */
	void ReceiveEndComponentOverlap(PrimitiveComponent* otherComp);

	// TODO: Place it in the UpdateOverlaps()
	bool PushOverlappingComponent(PrimitiveComponent* otherComponent, const OverlapInfo& overlapInfo) {
		auto [it, res] = currentlyOverlappingComponents.insert({ otherComponent, overlapInfo });
		return res;
	}

	bool PopOverlappingComponent(PrimitiveComponent* otherComponent) {
		std::size_t n = currentlyOverlappingComponents.erase(otherComponent);
		return n;
	}

	/**
	 * @brief 오버랩 스테이트를 업데이트 합니다.
	 */
	virtual void UpdateOverlaps(const std::vector<OverlapInfo>* overlaps, bool bDoNotifies = true);

	bool IsSimulatingPhysics() override {
		return bSimulatePhysics;
	}

	virtual DXVec2 GetComponentVelocity() const override {
		return Super::GetComponentVelocity();
	}

	// Bounds
	
	/**
	 * @brief Calculate the bounds on the world coordinate system.
	 * @param _worldTransform 
	 * @return 
	 */
	virtual BoxCircleBounds CalculateBounds(const Math::Matrix& _worldTransform) const override {
		BoxCircleBounds localBounds = CalculateLocalBounds();

		DXVec2 c{ localBounds.center.x, localBounds.center.y };
		c = DXVec2::Transform(c, _worldTransform);

		DXVec2 e{ localBounds.boxExtent.width, localBounds.boxExtent.height };
		e = DXVec2::Transform(e, _worldTransform);

		return BoxCircleBounds(Box{ c, e });
	}

	/**
	 * @brief Calculate the bounds without world transformation.
	 * @return 
	 */
	virtual BoxCircleBounds CalculateLocalBounds() const {
		return BoxCircleBounds{};
	}

	/**
	 * @brief Update the size of the bounds.
	 */
	virtual void UpdateBounds() override {
		bounds = CalculateBounds(GetWorldTransform());
	}

	// Physics update
	virtual void FixedUpdate(float _dt) override {
		
		if (bShouldOverlapTest)
		{
			UpdateOverlaps(nullptr, true);
			bShouldOverlapTest = false;
		}

		if (bSimulatePhysics) 
		{
			if (bTeleported)
			{
				MoveComponent(teleportDelta, angularVelocity * _dt, true, nullptr, true);
				bTeleported = false;
			} 
			else 
			{
				// Calculate drag force
				dragForce = velocity.LengthSquared() * frictionCoefficient;
				DXVec2 velDir = velocity; velDir.Normalize();
				acceleration += velDir * -dragForce; // Damping effect
				// Clamp acceleration
				DXVec2 accDir = acceleration; accDir.Normalize();
				float accClamped = Clamp(acceleration.Length(), minAcceleration, maxAccelaration);
				acceleration = accDir * accClamped;
				// Update velocity
				velocity += acceleration * _dt;
				acceleration = { 0, 0 }; // Reset acceleration each frame

				// Clamp velocity
				float speed = velocity.Length();
				DXVec2 unitVel = velocity; unitVel.Normalize();
				speed = Clamp(speed, minSpeed, maxSpeed);
				velocity = unitVel * speed;

				// Sweep to move
				HitResult hitResult;
				MoveComponent(velocity * _dt, angularVelocity * _dt, true, &hitResult);

				if (hitResult.bBlockingHit && hitResult.bStartPenetrating) {
					Translate(hitResult.normal * hitResult.penetrationDepth * 1.1f);
				}
			}
		}
		else 
		{
			if (bTeleported)
			{
				MoveComponent(teleportDelta, angularVelocity * _dt, true, nullptr, true);
				bTeleported = false;
			}
			else
			{
				// Clamp velocity
				float speed = velocity.Length();
				DXVec2 unitVel = velocity; unitVel.Normalize();
				speed = Clamp(speed, minSpeed, maxSpeed);
				velocity = unitVel * speed;

				// Apply the movement
				HitResult hitResult;
				MoveComponent(velocity * _dt, angularVelocity * _dt, false, &hitResult);

				if (hitResult.bBlockingHit && hitResult.bStartPenetrating) {
					Translate(hitResult.normal * hitResult.penetrationDepth * 1.1f);
				}
			}
		}
	}

	static void PullBackHit(HitResult& Hit, const DXVec2& Start, const DXVec2& End, const float Dist)
	{
		const float DesiredTimeBack = Clamp(0.1f, 0.1f / Dist, 1.f / Dist) + 0.001f;
		Hit.time = Clamp(Hit.time - DesiredTimeBack, 0.f, 1.f);
	}

protected:

	virtual bool CheckComponentOverlapComponentImpl(
		PrimitiveComponent* primComp,
		const DXVec2& pos, 
		const DXMat4x4& rotation);

	virtual bool CheckComponentOverlapComponentWithResultImpl(
		PrimitiveComponent* component, 
		const DXVec2& pos, const DXMat4x4& rotation,
		std::vector<OverlapResult>& outOverlap);

	virtual bool MoveComponentImpl(
		const DXVec2& delta,
		const float angleDelta,
		bool bSweep,
		HitResult* outHitResult,
		bool teleport) override;

};

// TODO: Line trace, sphere trace, sphere overlap

template<class T>
concept PrimitiveComponentType = std::is_base_of<PrimitiveComponent, T>::value;
