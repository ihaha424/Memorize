#pragma once
#include "IObject.h"

class IComponent : public IObject {
	class Actor* owner{ nullptr };

public:
	using ComponentTagRegistry = std::unordered_set<std::string>;
	ComponentTagRegistry componentTags;

	IComponent() {
		EnableTickEverySecond(false);
	}
	virtual ~IComponent() {}

	virtual void BeginPlay() {} ;
	void SetOwner(class Actor* _owner) { owner = _owner; }
	class Actor* GetOwner() const { return owner; }
	class World* GetWorld() const;
};

template<class T>
concept ComponentType = std::is_base_of<IComponent, T>::value;

