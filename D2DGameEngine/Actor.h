#pragma once
#include "IObject.h"

enum EActorStatus
{
	AS_AWAKE, AS_INACTIVE, AS_ACTIVE, AS_DESTROY
};


class Actor : public IObject
{
private:
	std::vector<class IComponent*> components;
	EActorStatus status = AS_AWAKE;

protected:
	class SceneComponent* rootComponent = nullptr;

public:
	Actor();
	virtual ~Actor();

	/**
	 * @brief Actor의 상태를 AS_ACTIVE로 변경합니다. 
	 */
	void Activate() { status = AS_ACTIVE; }

	/**
	 * @brief Actor의 상태를 AS_INACTIVE로 변경합니다. 
	 */
	void Inactivate() { status = AS_INACTIVE; }

	void SetLocation(const Math::Vector2 _location) { if(rootComponent != nullptr) rootComponent->SetWorldLocation(_location); }
	Math::Vector2 GetLocation() const { if (rootComponent != nullptr) return rootComponent->GetWorldLocation(); }
	void SetRotation(const Math::Vector2 _rotation) { if (rootComponent != nullptr) rootComponent->SetWorldRotation(_rotation); }
	Math::Vector2 GetRotation() const { if (rootComponent != nullptr) return rootComponent->GetWorldRotation(); }
	void SetScale(const float _scale) { if (rootComponent != nullptr) rootComponent->SetWorldScale(_scale); }
	Math::Vector2 GetScale() const { if (rootComponent != nullptr) return rootComponent->GetWorldScale(); }
	void SetTransform(const D2D_TMat3x2F _transform != nullptr) { if (rootComponent) rootComponent->SetWorldTransform(_transform); }
	D2D_TMat3x2F GetTrasnform() const { if (rootComponent != nullptr) return rootComponent->GetWorldTransform(); }

	/**
	 * @brief 특정 타입의 컴포넌트를 가져옵니다.
	 * @tparam T 컴포넌트 타입
	 * @return 컴포넌트 포인터. 찾을 수 없으면 nullptr
	 */
	template <typename T>
	T* GetComponent()
	{
		for (auto component : components)
		{
			T* t = dynamic_cast<T*>(component);
			if (t != nullptr)
				return t;
		}
		return nullptr;
	}

	/**
	 * @brief Actor의 상태를 AS_AWAKE에서 AS_ACTIVE로 변경합니다.
	 *		  레벨이 시작될 때 호출됩니다. 
	 */
	virtual void BeginPlay() { status = AS_ACTIVE; };

	/**
	 * @brief Actor의 상태를 AS_DESTROY로 변경합니다. 
	 */
	virtual bool Destroy();

	virtual void FixedUpdate(float _fixedRate) override;
	virtual void PreUpdate(float _dt) override;
	virtual void Update(float _dt) override;
	virtual void PostUpdate(float _dt) override;
	virtual void Render(class D2DRenderer* _renderer) override;

};

