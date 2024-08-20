#pragma once

#include "IEffect.h"

class EffectSystem
{
public:
	EffectSystem() {};
	~EffectSystem() {};

	template<IEffectType T>
	T* CreateEffect()
	{
		T* newEffect = new T(world);
		effectList.push_back(newEffect);
		return newEffect;
	}

	virtual void BeginPlay();
	virtual void FixedUpdate(float _fixedRate);
	virtual void Update(float _dt);
	virtual void Render(class D2DRenderer* _renderer);
	virtual void Destroy();
	virtual void CleanUp();
	virtual void Clean();

	void SetWorld(class World* _world) { world = _world; }
	class World* GetWorld() { return world; }

private:
	std::list<IEffect*> effectList {};
	class World* world = nullptr;
};

