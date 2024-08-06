#pragma once

#include "framework.h"

#include "IObject.h"
#include "Actor.h"

class IComponent : public IObject {
	Actor* owner{ nullptr };

public:
	IComponent() {}
	virtual ~IComponent() {}

	void SetOwner(Actor* _owner) { owner = _owner; }
	Actor* GetOwner() const { return owner; }
	class World* GetWorld() { return owner->GetWorld(); }
};

template<class T>
concept ComponentType = std::is_base_of<IComponent, T>::value;

