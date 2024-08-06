#pragma once

#include "IComponent.h"

#include "HitResult.h"

class SceneComponent : public IComponent {
	
protected:
	SceneComponent* parent;
	std::list<SceneComponent*> children;

	D2D_Mat3x2F S;
	D2D_Mat3x2F R;
	D2D_Mat3x2F T;
	
	// TODO: Gizmo

	// TODO: Bounds BoxSphereBounds

	// Component Velocity
	DXVec2 velocity{ 0.f, 0.f };
public:

	SceneComponent() :
		parent{ nullptr },
		S{ D2D_Mat3x2F::Identity() },
		R{ D2D_Mat3x2F::Identity() },
		T{ D2D_Mat3x2F::Identity() } {
		SetTickProperties(TICK_UPDATE);
	}

	void AddChild(SceneComponent* child);

	void RemoveChild(SceneComponent* child);

	virtual D2D_Mat3x2F GetWorldTransform() const;

	D2D_Point2F GetWorldPosition() const {
		D2D_Point2F p{};
		p = p * GetWorldTransform();
		return p;
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

	DXVec2 GetComponentVelocity() const {
		return velocity;
	}

	bool MoveComponent(
		const DXVec2& delta,
		bool bSweep,
		HitResult* outHitResult = nullptr) {
		return MoveComponentImpl(delta, bSweep, outHitResult);
	}

	// Collision
	// vitual CollisionEnabled::Type GetCollisionEnabled() {}

	virtual bool IsSimulatingPhysics() {
		return false;
	}

	// Tick
	virtual void Update(float _dt) override {
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
