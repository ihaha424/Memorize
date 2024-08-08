#pragma once

#include "IComponent.h"

#include "CollisionEnabled.h"
#include "ObjectType.h"
#include "BoxCircleBounds.h"
#include "HitResult.h"

/**
 * @brief Has transform and hierarchy but no rendering and collision
 */
class SceneComponent : public IComponent {
	
public:

	SceneComponent* parent;
	std::list<SceneComponent*> children;

	// Bounds
	bool bShouldUpdateBounds{ false };
	BoxCircleBounds bounds;

protected:

	Math::Matrix S;
	Math::Matrix R;
	Math::Matrix T;

	// Component Velocity
	DXVec2 velocity{ 0.f, 0.f };
public:

	SceneComponent(Actor* _owner) :
		IComponent(_owner), parent{ nullptr }, S{}, R{}, T{} {
		SetTickProperties(TICK_PHYSICS);
	}

	void AddChild(SceneComponent* child);

	void RemoveChild(SceneComponent* child);

	virtual Math::Matrix GetWorldTransform() const;

	Math::Vector2 GetComponentLocation() const {
		DXVec3 p = DXVec3::Transform(DXVec3::Zero, GetWorldTransform());
		return DXVec2(p);
	}

	Math::Vector2 GetRightVector() const {
		DXVec3 p = DXVec3::Transform(DXVec3::Right, GetWorldTransform());
		return DXVec2(p);
	}

	Math::Vector2 GetUpVector() const {
		DXVec3 p = DXVec3::Transform(DXVec3::Up, GetWorldTransform());
		return DXVec2(p);
	}

	void SetScale(float x, float y) {
		S = DXMat4x4::CreateScale(x, y, 1.0);
	}

	void SetRotation(float degree) {
		R = DXMat4x4::CreateRotationZ(Math::DegreeToRadian(degree));
	}

	void SetTranslation(float x, float y) {
		T = DXMat4x4::CreateTranslation(x, y, 0.f);
	}

	void Scale(float dx, float dy) {
		S = S * DXMat4x4::CreateScale(dx, dy, 1.0);
	}

	void Rotate(float degree) {
		R = R * DXMat4x4::CreateRotationZ(Math::DegreeToRadian(degree));
	}

	void Translate(float dx, float dy) {
		T = T * DXMat4x4::CreateTranslation(dx, dy, 0.f);
	}

	void Translate(const DXVec2& dv) {
		T = T * DXMat4x4::CreateTranslation(dv.x, dv.y, 0.f);
	}

	void SetComponentVelocity(Math::Vector2 _velocity) {
		velocity = velocity;
	}

	void AddComponentVelocity(Math::Vector2 _velocity) {
		velocity += _velocity;
	}

	bool MoveComponent(
		const Math::Vector2& delta,
		bool bSweep,
		HitResult* outHitResult = nullptr) {
		return MoveComponentImpl(delta, bSweep, outHitResult);
	}

	// Collision
	virtual bool IsCollisionEnabled() const {
		return false;
	}

	virtual CollisionEnabled::Type GetCollisionEnabled() const {
		return CollisionEnabled::NoCollision;
	}

	virtual ECollisionChannel GetCollisionObjectType() const {
		return ECollisionChannel::WorldStatic;
	}

	// Bounds
	virtual BoxCircleBounds CalculateBounds(const Math::Matrix& _worldTransform) const {
		return BoxCircleBounds{};
	}

	virtual BoxCircleBounds CalculateLocalBounds() const {
		return BoxCircleBounds{};
	}

	virtual void UpdateBounds() {}


	// Overlap states
	
	/**
	 * @brief 이 함수 사용시 GetCollisionEnabled 체크가 필요합니다.
	 */
	virtual void UpdateOverlaps() {}

	virtual bool IsSimulatingPhysics() {
		return false;
	}

	virtual Math::Vector2 GetComponentVelocity() const {
		return velocity;
	}

	// TODO: Destroy Component

	// Tick
	virtual void FixedUpdate(float _dt) override {
		MoveComponent(velocity * _dt, false);
	}

protected:

	virtual bool MoveComponentImpl(
		const Math::Vector2& delta,
		bool bSweep,
		HitResult* outHitResult) {
		Translate(delta);
		outHitResult = {};
		return true;
	}

};

template<class T>
concept SceneComponentType = std::is_base_of<SceneComponent, T>::value;
