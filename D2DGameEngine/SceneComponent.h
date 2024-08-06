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

	// Physics setting
	bool enablePhysics{ false };
public:

	SceneComponent() :
		parent{ nullptr },
		S{ D2D_Mat3x2F::Identity() },
		R{ D2D_Mat3x2F::Identity() },
		T{ D2D_Mat3x2F::Identity() } {}

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

	void Translate(D2D_Vec2F dv) {
		T = T * D2D_Mat3x2F::Translation(dv.x, dv.y);
	}

	bool MoveComponent(
		const D2D_Vec2F& delta, 
		bool bSweep,
		HitResult& outHitResult) {
		return MoveComponentImpl(delta, bSweep, outHitResult);
	}

	bool IsSimulatingPhysics() {
		return enablePhysics;
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

template<class T>
concept SceneComponentType = std::is_base_of<SceneComponent, T>::value;
