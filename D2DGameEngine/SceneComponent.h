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

	D2D_Mat3x2F S;
	D2D_Mat3x2F R;
	D2D_Mat3x2F T;

	// Component Velocity
	DXVec2 velocity{ 0.f, 0.f };
public:

	SceneComponent() :
		parent{ nullptr },
		S{ D2D_Mat3x2F::Identity() },
		R{ D2D_Mat3x2F::Identity() },
		T{ D2D_Mat3x2F::Identity() } {
		SetTickProperties(TICK_PHYSICS);
	}

	void AddChild(SceneComponent* child);

	void RemoveChild(SceneComponent* child);

	virtual D2D_Mat3x2F GetWorldTransform() const;

	D2D_Point2F GetComponentLocation() const {
		D2D_Point2F p{};
		p = p * GetWorldTransform();
		return p;
	}

	D2D_Vec2F GetRightVector() const {
		D2D_Point2F p{ 1.f, 0.f };
		p = p * GetWorldTransform();
		return { p.x, p.y };
	}

	D2D_Vec2F GetUpVector() const {
		D2D_Point2F p{ 0.f, 1.f };
		p = p * GetWorldTransform();
		return { p.x, p.y };
	}

	void SetScale(float x, float y) {
		S = D2D_Mat3x2F::Scale(x, y);
	}

	void SetRotation(float degree) {
		R = D2D_Mat3x2F::Rotation(degree);
	}

	void SetTranslation(float x, float y) {
		T = D2D_Mat3x2F::Translation(x, y);
	}

	void Scale(float dx, float dy) {
		S = S * D2D_Mat3x2F::Scale(dx, dy);
	}

	void Rotate(float degree) {
		R = R * D2D_Mat3x2F::Rotation(degree);
	}

	void Translate(float dx, float dy) {
		T = T * D2D_Mat3x2F::Translation(dx, dy);
	}

	void Translate(DXVec2 dv) {
		T = T * D2D_Mat3x2F::Translation(dv.x, dv.y);
	}

	void SetComponentVelocity(DXVec2 _velocity) {
		velocity = velocity;
	}

	void AddComponentVelocity(DXVec2 _velocity) {
		velocity += _velocity;
	}

	bool MoveComponent(
		const DXVec2& delta,
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
	virtual BoxCircleBounds CalculateBounds(const D2D_Mat3x2F& _worldTransform) const {
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

	virtual DXVec2 GetComponentVelocity() const {
		return velocity;
	}

	// TODO: Destroy Component

	// Tick
	virtual void FixedUpdate(float _dt) override {
		MoveComponent(velocity * _dt, false);
	}

protected:

	virtual bool MoveComponentImpl(
		const DXVec2& delta,
		bool bSweep,
		HitResult* outHitResult) {
		Translate(delta);
		outHitResult = {};
		return false;
	}

};

template<class T>
concept SceneComponentType = std::is_base_of<SceneComponent, T>::value;
