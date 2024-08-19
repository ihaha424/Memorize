#pragma once
#include "Actor.h"

class IEffect 
	: public Actor
{
public:
	IEffect(class World* _world) :Actor(_world) { SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER); }
	virtual ~IEffect() {};

	virtual void FixedUpdate(float _fixedRate);

	void SetAliveTime(float _aliveTime) { aliveTime = _aliveTime; }

protected:
	float aliveTime{0.f};
};

template<class T>
concept IEffectType = std::is_base_of<IEffect, T>::value;

//// 시간
//움직임
//이미지