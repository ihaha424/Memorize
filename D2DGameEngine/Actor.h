#pragma once
#include "IComponent.h"
#include "PrimitiveComponent.h"

#include "HitResult.h"

class Actor : public IObject
{
protected:
	class World* world = nullptr;

public: 
	using ComponentRegistry = std::unordered_multimap<std::type_index, class IComponent*>;
	ComponentRegistry components;

	using ComponentRenderSequence = std::multimap<float, class PrimitiveComponent*>;
	ComponentRenderSequence renderSequence;

	bool bRenderDirty{ false };
	int	renderLayer = 2;

	class SceneComponent* rootComponent = nullptr;
	Actor(class World* _world);
	virtual ~Actor();

	void SetWorld(class World* _world) { world = _world; }
	class World* GetWorld() { return world; }

	/**
	 * @brief Actor�� ���¸� AS_ACTIVE�� �����մϴ�. 
	 */
	void Activate() { status = OS_ACTIVE; }

	/**
	 * @brief Actor�� ���¸� AS_INACTIVE�� �����մϴ�. 
	 */
	void Inactivate() { status = OS_INACTIVE; }

	Math::Vector2 Right() {
		if (rootComponent != nullptr)
			rootComponent->GetRightVector();
	}

	Math::Vector2 Up() {
		if (rootComponent != nullptr)
			rootComponent->GetUpVector();
	}

	void SetLocation(const float x, const float y);
	Math::Vector2 GetLocation() const;
	void SetRotation(const float _degree);
	void SetScale(const float x, const float y);
	Math::Matrix GetTrasnform() const;

	void RotateToward(const Math::Vector2& _direction);
	void LookAt(const Math::Vector2& _point);

	/**
	 * @brief Translate and overlap test.
	 * @param _teleportDelta The amount of position to be moved from the current position.
	 */
	void Teleport(const DXVec2& _teleportDelta) {
		if (rootComponent)
			rootComponent->TeleportComponent(_teleportDelta);
	}
	/**
	 * @brief Set velocity of the component
	 * @param _velocity 
	 */
	void SetVelocity(Math::Vector2 _velocity) {
		if (rootComponent)
			rootComponent->SetComponentVelocity(_velocity);
	}
	/**
	 * @brief Add velocity of the component
	 * @param _velocity
	 */
	void AddVelocity(Math::Vector2 _velocity) {
		if (rootComponent)
			rootComponent->AddComponentVelocity(_velocity);
	}
	/**
	 * @brief Set angular velocity of the component
	 * @param _angularVelocity Positive angle or negative angle in degree.
	 */
	void SetAngularVelocity(float _angularVelocity) {
		if (rootComponent)
			rootComponent->SetComponentAngularVelocity(_angularVelocity);
	}
	/**
	 * @brief Add angular velocity of the component
	 * @param _angularVelocity Positive angle or negative angle in degree.
	 */
	void AddAngularVelocity(float _angularVelocity) {
		if (rootComponent)
			rootComponent->AddComponentAngularVelocity(_angularVelocity);
	}

	template<ComponentType T>
	requires (!PrimitiveComponentType<T>)
	T* CreateComponent()
	{
		T* component = new T(this);
		components.insert({ std::type_index(typeid(T)), component });
		return component;
	}

	template<PrimitiveComponentType T>
	T* CreateComponent() 
	{
		T* component = new T(this);
		renderSequence.insert({ component->GetComponentLocation().y, component });
		components.insert({ std::type_index(typeid(T)), component });
		return component;
	}

	/**
	 * @brief Ư�� Ÿ���� ������Ʈ�� �����ɴϴ�.
	 * @tparam T ������Ʈ Ÿ��
	 * @return ������Ʈ ������. ã�� �� ������ nullptr
	 */
	template <ComponentType T>
	T* GetComponent() {
		auto it = components.find(std::type_index(typeid(T)));
		return (it == components.end()) ? nullptr : (T*)it->second;
	}


	// Collision Event
	void NotifyActorBeginOverlap(Actor* other, const OverlapInfo& overlap) {
		OnBeginOverlap(other, overlap);
	}
	void NotifyActorOverlap(Actor* other, const OverlapInfo& overlap) {
		OnOverlap(other, overlap);
	}
	void NotifyActorEndOverlap(Actor* other, const OverlapInfo& overlap) {
		OnEndOverlap(other, overlap);
	}
	void NotifyBlockingHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) {
		OnHit(myComp, otherComp, bSelfMoved, hitResult);
	}

	/**
	 * @brief ClickComponent Evnet
	 */
	virtual void OnBeginCursorOver() {};
	virtual void OnEndCursorOver() {};
	virtual void OnClicked() {};
	virtual void OnPressed() {};

	/**
	 * @brief 
	 * @param other 
	 * @param overlap 
	 */
	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) {}

	/**
	 * @brief 
	 * @param other 
	 * @param overlap 
	 */
	virtual void OnOverlap(Actor* other, const OverlapInfo& overlap) {}

	/**
	 * @brief 
	 * @param other 
	 * @param overlap 
	 */
	virtual void OnEndOverlap(Actor* other, const OverlapInfo& overlap) {}
	/**
	 * @brief TODO;
	 * @param myComp 
	 * @param otherComp 
	 * @param bSelfMoved 
	 * @param hitResult 
	 */
	virtual void OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) {}

	/**
	 * @brief ������ �ޱ�. ���� ���, �Ѿ��� OnHit �̺�Ʈ���� otherComp�� ���ʸ� ������ �� TakeDamage�� ȣ�� �� �� �ֽ��ϴ�.
	 * @param damageAmount 
	 * @param damageEvent 
	 * @param eventInstigator 
	 * @param damageCauser 
	 * @return 
	 */
	float TakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser);
	/**
	 * @brief �������� ��ǻ� �󸶳� �޾Ҵ��� ���. TakeDamage�� ����
	 * @param damageAmount 
	 * @param damageEvent 
	 * @param eventInstigator 
	 * @param damageCauser 
	 * @return 
	 */
	virtual float InternalTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) { return damageAmount; }
	/**
	 * @brief ������ �޾��� �� �̺�Ʈ. TakeDamage �ȿ��� ȣ���.
	 * @param damageAmount 
	 * @param damageEvent 
	 * @param eventInstigator 
	 * @param damageCauser 
	 */
	virtual void OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) {}

	/**
	 * @brief Actor�� ���¸� AS_AWAKE���� AS_ACTIVE�� �����մϴ�.
	 *		  ������ ���۵� �� ȣ��˴ϴ�. 
	 */
	virtual void BeginPlay();

	/**
	 * @brief Actor�� ���¸� AS_DESTROY�� �����մϴ�. 
	 */
	virtual bool Destroy();

	virtual void FixedUpdate(float _fixedRate) override;
	virtual void PreUpdate(float _dt) override;
	virtual void Update(float _dt) override;
	virtual void PostUpdate(float _dt) override;
	virtual void PrepareRender();
	virtual void Render(class D2DRenderer* _renderer) override;

};

template<class T>
concept ActorType = std::is_base_of<Actor, T>::value;

