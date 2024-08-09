#pragma once
#include "IComponent.h"

#include "HitResult.h"

class PrimitiveComponent;
class Actor : public IObject
{
protected:
	using ComponentRegistry = std::unordered_multimap<std::type_index, class IComponent*>;
	ComponentRegistry components;

	class World* world = nullptr;

public: 
	class SceneComponent* rootComponent = nullptr;
	Actor(class World* _world);
	virtual ~Actor();

	void SetWorld(class World* _world) { world = _world; }
	class World* GetWorld() { return world; }

	/**
	 * @brief Actor의 상태를 AS_ACTIVE로 변경합니다. 
	 */
	void Activate() { status = OS_ACTIVE; }

	/**
	 * @brief Actor의 상태를 AS_INACTIVE로 변경합니다. 
	 */
	void Inactivate() { status = OS_INACTIVE; }

	void SetLocation(const float x, const float y);
	Math::Vector2 GetLocation() const;
	void SetRotation(const float _degree);
	void SetScale(const float x, const float y);
	Math::Matrix GetTrasnform() const;

	template<ComponentType T>
	T* CreateComponent()
	{
		T* component = new T(this);
		components.insert({ std::type_index(typeid(T)), component });
		return component;
	}

	/**
	 * @brief 특정 타입의 컴포넌트를 가져옵니다.
	 * @tparam T 컴포넌트 타입
	 * @return 컴포넌트 포인터. 찾을 수 없으면 nullptr
	 */
	template <typename T>
	T* GetComponent() {
		auto it = components.find(std::type_index(typeid(T)));
		return (it == components.end()) ? nullptr : (T*)it->second;
	}


	// Collision Event
	void NotifyActorBeginOverlap(Actor* other) {
		OnBeginOverlap(other);
	}
	void NotifyActorEndOverlap(Actor* other) {
		OnEndOverlap(other);
	}
	void NotifyBlockingHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) {
		OnHit(myComp, otherComp, bSelfMoved, hitResult);
	}

	virtual void OnBeginOverlap(Actor* other) {}
	virtual void OnEndOverlap(Actor* other) {}
	virtual void OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) {}

	/**
	 * @brief 데미지 받기. 예를 들어, 총알의 OnHit 이벤트에서 otherComp의 오너를 가져온 뒤 TakeDamage를 호출 할 수 있습니다.
	 * @param damageAmount 
	 * @param damageEvent 
	 * @param eventInstigator 
	 * @param damageCauser 
	 * @return 
	 */
	float TakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser);
	/**
	 * @brief 데미지를 사실상 얼마나 받았는지 계산. TakeDamage의 헬퍼
	 * @param damageAmount 
	 * @param damageEvent 
	 * @param eventInstigator 
	 * @param damageCauser 
	 * @return 
	 */
	virtual float InternalTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) { return 0.f; }
	/**
	 * @brief 데미지 받았을 때 이벤트. TakeDamage 안에서 호출됨.
	 * @param damageAmount 
	 * @param damageEvent 
	 * @param eventInstigator 
	 * @param damageCauser 
	 * @param hitResult 
	 */
	virtual void OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) {}

	/**
	 * @brief Actor의 상태를 AS_AWAKE에서 AS_ACTIVE로 변경합니다.
	 *		  레벨이 시작될 때 호출됩니다. 
	 */
	virtual void BeginPlay();

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

template<class T>
concept ActorType = std::is_base_of<Actor, T>::value;

